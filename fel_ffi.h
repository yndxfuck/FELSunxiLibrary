/* For Dart FFI */

void load_spl(void *raw, size_t size, uint8_t *buf);
int write_to_memory(void *raw, size_t offset, size_t size, void *buf);
int reboot_device(void *raw);
void *open_feldev_handle();
void close_feldev_handle(void *raw);
