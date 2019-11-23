const unsigned char power_up[] = {
0x20, 0x00,
0x20, 0x00,
0x01,
0xff,0xff,0xff,0xff,0xff,0x40,0x40,0xcb,0xcb,0xcb,0xcb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x40,0x40,0xcb,0xcb,0xcb,0xcb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x40,0x40,0xcb,0xcb,0x3d,0x40,0xe8,0xe8,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0x40,0x40,0xcb,0xcb,0x3d,0x40,0xe8,0xe8,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0x40,0xcb,0xcb,0xcb,0xcb,0x3d,0x40,0xe8,0xe8,0xe8,0xe8,0xdf,0xff,0xff,0xff,0xff,0x40,0xcb,0xcb,0xcb,0xcb,0x3d,0x40,0xe8,0xe8,0xe8,0xe8,0xdf,0xff,0xff,0xff,0x40,0xcb,0xcb,0xcb,0xcb,0xcb,0x3d,0x40,0xdf,0xe8,0xe8,0xe8,0xe8,0xdf,0xff,0xff,0x40,0xcb,0xcb,0xcb,0xcb,0xcb,0x3d,0x40,0xdf,0xe8,0xe8,0xe8,0xe8,0xdf,0xff,0xff,0x40,0xcb,0xcb,0xcb,0xcb,0x75,0x75,0x75,0x7c,0xdf,0xe8,0xe8,0xe8,0xdf,0xff,0xff,0x40,0xcb,0xcb,0xcb,0xcb,0x7d,0x7d,0x7d,0x7d,0xdf,0xe8,0xe8,0xe8,0xdf,0xff,0x40,0xcb,0xcb,0xcb,0xcb,0x75,0x75,0x75,0x75,0x7c,0x7c,0xdf,0xe8,0xe8,0xe8,0xdf,0x40,0xcb,0xcb,0xcb,0xcb,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0xdf,0xe8,0xe8,0xe8,0xdf,0x40,0xcb,0xcb,0xcb,0x75,0x75,0x40,0x40,0x75,0x75,0x7c,0x7c,0xdf,0xe8,0xe8,0xdf,0x40,0xcb,0xcb,0xcb,0x7d,0x7d,0x75,0x75,0x7d,0x7d,0x7d,0x7d,0xdf,0xe8,0xe8,0xdf,0xcb,0xe8,0xe8,0xe8,0x75,0x75,0x40,0x40,0x75,0x75,0x7c,0x7c,0xdf,0xdf,0xdf,0xdf,0xcb,0xe8,0xe8,0xe8,0x7d,0x7d,0x75,0x75,0x7d,0x7d,0x7d,0x7d,0xdf,0xdf,0xdf,0xdf,0xcb,0xcb,0xcb,0xcb,0x7c,0x75,0x75,0x75,0x75,0x75,0x7c,0x7c,0x3d,0x3d,0x3d,0xdf,0xcb,0xcb,0xcb,0xcb,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0x3d,0x3d,0x3d,0xdf,0xcb,0x3d,0x3d,0x3d,0x7c,0x7c,0x75,0x75,0x75,0x7c,0x7c,0x7c,0xdf,0xdf,0xdf,0xdf,0xcb,0x3d,0x3d,0x3d,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0xdf,0xdf,0xdf,0xdf,0xcb,0x3d,0x3d,0x3d,0x7c,0x7c,0x7c,0x7c,0x7c,0x7c,0x7c,0xdf,0xdf,0xdf,0xdf,0xdf,0xcb,0x3d,0x3d,0x3d,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0x7d,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xcb,0x3d,0x3d,0x3d,0xe8,0x7c,0x7c,0x7c,0x7c,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xcb,0x3d,0x3d,0x3d,0xe8,0x7d,0x7d,0x7d,0x7d,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xcb,0x3d,0x3d,0x3d,0x3d,0xe8,0xdf,0x3d,0xdf,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xcb,0x3d,0x3d,0x3d,0x3d,0xe8,0xdf,0x3d,0xdf,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xff,0xcb,0x3d,0x3d,0x3d,0x3d,0xdf,0x3d,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xff,0xff,0xcb,0x3d,0x3d,0x3d,0x3d,0xdf,0x3d,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0xcb,0xcb,0x3d,0x3d,0xdf,0x3d,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xcb,0xcb,0x3d,0x3d,0xdf,0x3d,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdf,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdf,0xdf,0xdf,0xdf,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0x12,0x12,0x12,0x12,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0x12,0x12,0x12,0x12,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0x12,0x12,0x18,0xc0,0x1a,0x1a,0x1e,0x1e,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0x12,0x12,0x18,0xc0,0x1a,0x1a,0x1e,0x1e,0xff,0xff,0xff,0xff,0xff,0xc0,0x12,0x12,0x12,0x12,0x18,0xc0,0x1a,0x1a,0x1a,0x1a,0x1e,0xff,0xff,0xff,0xff,0xc0,0x12,0x12,0x12,0x12,0x18,0xc0,0x1a,0x1a,0x1a,0x1a,0x1e,0xff,0xff,0xff,0xc0,0x12,0x12,0x12,0x12,0x12,0x18,0xc0,0x1e,0x1a,0x1a,0x1a,0x1a,0x1e,0xff,0xff,0xc0,0x12,0x12,0x12,0x12,0x12,0x18,0xc0,0x1e,0x1a,0x1a,0x1a,0x1a,0x1e,0xff,0xff,0xc0,0x12,0x12,0x12,0x12,0xcb,0xcb,0xcb,0x3d,0x1e,0x1a,0x1a,0x1a,0x1e,0xff,0xff,0xc0,0x12,0x12,0x12,0x12,0xe8,0xe8,0xe8,0xe8,0x1e,0x1a,0x1a,0x1a,0x1e,0xff,0xc0,0x12,0x12,0x12,0x12,0xcb,0xcb,0xcb,0xcb,0x3d,0x3d,0x1e,0x1a,0x1a,0x1a,0x1e,0xc0,0x12,0x12,0x12,0x12,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0x1e,0x1a,0x1a,0x1a,0x1e,0xc0,0x12,0x12,0x12,0xcb,0xcb,0x40,0x40,0xcb,0xcb,0x3d,0x3d,0x1e,0x1a,0x1a,0x1e,0xc0,0x12,0x12,0x12,0xe8,0xe8,0xcb,0xcb,0xe8,0xe8,0xe8,0xe8,0x1e,0x1a,0x1a,0x1e,0x12,0x1a,0x1a,0x1a,0xcb,0xcb,0x40,0x40,0xcb,0xcb,0x3d,0x3d,0x1e,0x1e,0x1e,0x1e,0x12,0x1a,0x1a,0x1a,0xe8,0xe8,0xcb,0xcb,0xe8,0xe8,0xe8,0xe8,0x1e,0x1e,0x1e,0x1e,0x12,0x12,0x12,0x12,0x3d,0xcb,0xcb,0xcb,0xcb,0xcb,0x3d,0x3d,0x18,0x18,0x18,0x1e,0x12,0x12,0x12,0x12,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0x18,0x18,0x18,0x1e,0x12,0x18,0x18,0x18,0x3d,0x3d,0xcb,0xcb,0xcb,0x3d,0x3d,0x3d,0x1e,0x1e,0x1e,0x1e,0x12,0x18,0x18,0x18,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0x1e,0x1e,0x1e,0x1e,0x12,0x18,0x18,0x18,0x1a,0x3d,0x3d,0x3d,0x3d,0x3d,0x3d,0x1e,0x1e,0x1e,0x1e,0x1e,0x12,0x18,0x18,0x18,0x1a,0xe8,0xe8,0xe8,0xe8,0xe8,0xe8,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0x12,0x18,0x18,0x18,0x1a,0x3d,0x3d,0x3d,0x3d,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0x12,0x18,0x18,0x18,0x1a,0xe8,0xe8,0xe8,0xe8,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0x12,0x18,0x18,0x18,0x18,0x1a,0x1e,0x18,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0x12,0x18,0x18,0x18,0x18,0x1a,0x1e,0x18,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0xff,0x12,0x18,0x18,0x18,0x18,0x1e,0x18,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0xff,0xff,0x12,0x18,0x18,0x18,0x18,0x1e,0x18,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0xff,0xff,0xff,0x12,0x12,0x18,0x18,0x1e,0x18,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0xff,0xff,0xff,0xff,0x12,0x12,0x18,0x18,0x1e,0x18,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0xff,0xff,0xff,0xff,0xff,};
