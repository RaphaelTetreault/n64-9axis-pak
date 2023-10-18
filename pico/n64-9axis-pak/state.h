enum state
{
  invalid,
  await_write, //0x03 - Write Controller Accessory
  await_read,  //0x02 - Read Controller Accessory
  complete_write,
  complete_read,
};