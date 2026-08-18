    .section .rodata,"a",@progbits

    .balign 16
    .global g_heiti_ttf_start
    .global g_heiti_ttf_end
    .hidden g_heiti_ttf_start
    .hidden g_heiti_ttf_end
    .type g_heiti_ttf_start, %object
g_heiti_ttf_start:
    .incbin "素材/heiti.ttf"
g_heiti_ttf_end:
    .size g_heiti_ttf_start, g_heiti_ttf_end - g_heiti_ttf_start

    .balign 16
    .global g_picture_jpg_start
    .global g_picture_jpg_end
    .hidden g_picture_jpg_start
    .hidden g_picture_jpg_end
    .type g_picture_jpg_start, %object
g_picture_jpg_start:
    .incbin "素材/picture.jpg"
g_picture_jpg_end:
    .size g_picture_jpg_start, g_picture_jpg_end - g_picture_jpg_start

    .balign 16
    .global g_congyv_gif_start
    .global g_congyv_gif_end
    .hidden g_congyv_gif_start
    .hidden g_congyv_gif_end
    .type g_congyv_gif_start, %object
g_congyv_gif_start:
    .incbin "素材/congyv.gif"
g_congyv_gif_end:
    .size g_congyv_gif_start, g_congyv_gif_end - g_congyv_gif_start

    .section .note.GNU-stack,"",@progbits
