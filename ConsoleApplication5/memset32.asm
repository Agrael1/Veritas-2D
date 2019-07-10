    page    ,132
    title   memset32 - set sections of memory all to one dword
;***
;memset32.asm - set a section of memory to all one dword
;
;
;Purpose:
;   contains the memset32() routine
;
;*******************************************************************************

include ksamd64.inc
        subttl  "memset32"
;***
;char *memset32(dst, value, count) - sets "count" bytes at "dst" to "value"
;
;Purpose:
;   Sets the first "count" dwords of the memory starting
;   at "dst" to the dword value "value".
;
;   Algorithm:
;   dword*
;   memset (dst, value, count)
;       {
;       dword *start = dst;
;
;       while (count--)
;           *dst++ = value;
;       return(start);
;       }
;
;Entry:
;   dword *dst - pointer to memory to fill with value (4 byte aligned preferred)
;   dword value - value to put in dst bytes
;   size_t count - number of bytes of dst to fill
;
;Exit:
;   returns dst, with filled bytes
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************
    extrn   __favor:dword
    extrn   __ImageBase:byte

__FAVOR_ENFSTRG equ 1

        LEAF_ENTRY_ARG3 memset32, _TEXT, buf:ptr dword, value:dword, count:dword

        OPTION PROLOGUE:NONE, EPILOGUE:NONE

        mov     r11, rcx                        ; save destination address

		cmp		r8, 4							; if count <= 3 use regular assignment
		jbe		SetUINTSmall

        mov     r9, 0000000100000001h           ; replicate fill over 8 bytes (2 dwords -> 1 qword)
        imul    r9, rdx

        movd    xmm0, r9                        ; bytes to store in bits [0:63]
        punpckldq xmm0, xmm0                    ; dup bytes to [127:64]

		cmp     r8, 32
		jbe     XmmSetSmall2                     ; if count <= 31 do not use Enhanced Fast Strings

;
; Check if Enhanced Fast Strings should be used.
;
        bt      __favor, __FAVOR_ENFSTRG
        jnc     XmmSet                          ; Enhanced Fast Strings not available, use SSE2

; Use Enhanced Fast Strings
        mov     eax, edx                        ; set byte to move
        mov     rdx, rdi                        ; save rdi
        mov     rdi, rcx                        ; set destination
        mov     rcx, r8                         ; set count
        rep     stosd                           ; store the uint
        mov     rdi, rdx                        ; restore rdi
        mov     rax, r11
        ret                                     ; return

;
; Fill using SSE instructions - size must be 16 or more.
;
        ; xmm0 has the byte to store replicated to all byte positions
        ; rcx has the destination, can be overwritten
        ; r11 has the destination, must be preserved for return value
        ; r8  has the count
        align   16
XmmSet:
        ; Aligned stores are much faster on AMD hardware. We need to do an unaligned
        ; store of (16 - (dest mod 16)) bytes, but it's faster to just store 16 bytes
        ; and then start the aligned loop as usual at ((dest + 16) - (dest mod 16)).
        ; This results in (dest mod 16) bytes being stored twice. This is a lot faster
        ; than a bunch of code to store maybe 8 then maybe 4 then maybe 2 then maybe 1
        ; byte to achieve alignement. It could cause data breakpoints to trigger twice,
        ; but they will hit here first and hopefully you will read this comment.
        ; The fastest way to subtract (16 - (dest mod 16)) from the length is to add
        ; (original dest - aligned dest). This avoids having to calculate the value.

		shl		r8, 2							; restore byte counter
        movups  [rcx], xmm0                     ; store 16 unaligned from start
        add     r8, rcx                         ; r8 = dest + length
        add     rcx, 16                         ; dest = (dest + 16)
        and     rcx, -16                        ; dest = (dest + 16) - (dest mod 16)
        sub     r8, rcx                         ; r8 = remaining length

; Attempt to set 128-byte blocks.
;
XmmSetLarge:
        mov     r9, r8                          ; copy count of bytes remaining
        shr     r9, 7                           ; compute number of 128-byte blocks
        jz      XmmSetSmall                     ; if z, no 128-byte blocks to fill
;
; Set 128-byte blocks
        align   16
XmmSetLargeLoop:
        movaps  0[rcx], xmm0
        movaps  16[rcx], xmm0
        add     rcx, 128                        ; advance destination address early
        movaps  (32 - 128)[rcx], xmm0
        movaps  (48 - 128)[rcx], xmm0
        dec     r9                              ; dec block counter (set cc for jnz)
        movaps  (64 - 128)[rcx], xmm0
        movaps  (80 - 128)[rcx], xmm0
        movaps  (96 - 128)[rcx], xmm0
        movapd  (112 - 128)[rcx], xmm0
        ; to avoid generating a one-byte NOP for the 'align 16' below the previous
        ; instruction is movapd instead of movaps which is one byte longer but
        ; performs exactly the same operation.
        jnz     XmmSetLargeLoop                 ; loop if more blocks

        and     r8, 127                         ; compute remaining byte count
		jmp XmmSetSmall							; because we don't need to double up the result once more
;
; Attempt to set 16-byte blocks
        align   16

XmmSetSmall2:
		shl		r8, 2
XmmSetSmall:
        mov     r9, r8                          ; copy count of bytes remaining
        shr     r9, 4                           ; compute number of 16-byte blocks
        jz      short XmmSetTrailing

        ; This generates an 8-byte nop, which we execute once. This will change only if
        ; any of the code from msetxmm30 down is modified. The following loop thus is
        ; completely contained within one instruction decode buffer on AMD hardware.
        align   16

;
; Set 16-byte blocks
;
XmmSetSmallLoop:
        movups  [rcx], xmm0
        add     rcx, 16
        dec     r9
        jnz     short XmmSetSmallLoop

XmmSetTrailing:
        and     r8, 15							; compute remaining length
        jz      XmmSetReturn                    ; skip over movups if done, we could just do it anyway

        ; As at the start, we are going to do an unaligned store of 16 bytes which will overwrite
        ; some bytes already stored. The math is easier, rcx+r8 is one byte past the end, just
        ; back up 16 from there and store 16.

        movups  [rcx+r8-16], xmm0               ; write remainder, overwriting 16-r8 bytes we already wrote

XmmSetReturn:
        mov     rax, r11                        ; must return original dest that we saved in r11
        ret

SetUINTSmall:
		mov		[rcx], edx
        add     rcx, 4
        dec     r8
        jnz     short SetUINTSmall

		mov     rax, r11
        ret                                     ; return

        LEAF_END memset32, _TEXT
    end