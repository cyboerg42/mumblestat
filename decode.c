int * decode_ping(char buf[]) {
  static int decoded[2];
  unsigned char a[4];
  a[0] = buf[15];
  a[1] = buf[14];
  a[2] = buf[13];
  a[3] = buf[12];
  unsigned char b[4];
  b[0] = buf[19];
  b[1] = buf[18];
  b[2] = buf[17];
  b[3] = buf[16];
  int users = *(int *)a;
  int maxusers = *(int *)b;
  decoded[0] = users;
  decoded[1] = maxusers;
  return decoded;
}
