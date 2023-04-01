#include <fcntl.h>
#include <stdint.h> 
#include <stdlib.h> 
#include <unistd.h>

typedef struct {
    uint64_t pfn : 54;
    unsigned int soft_dirty : 1;
    unsigned int file_page : 1;
    unsigned int swapped : 1;
    unsigned int present : 1;
} PagemapEntry;

int pagemap_get_entry(PagemapEntry *entry, int pagemap_fd, uintptr_t vaddr)
{
    size_t nread;
    ssize_t ret;
    uint64_t data;

    nread = 0;
    while (nread < sizeof(data)) {
        ret = pread(pagemap_fd, ((uint8_t*)&data) + nread, sizeof(data) - nread,
                (vaddr / sysconf(_SC_PAGE_SIZE)) * sizeof(data) + nread);
        nread += ret;
        if (ret <= 0) {
            return -1;
        }
    }
    entry->pfn = data & (((uint64_t)1 << 54) - 1);
    entry->soft_dirty = (data >> 54) & 1;
    entry->file_page = (data >> 61) & 1;
    entry->swapped = (data >> 62) & 1;
    entry->present = (data >> 63) & 1;
    return 0;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Not enough arguments. Required: [pid] [vaddr]");
		return -1;
	}
	char buf[256];
	if (strcmp(argv[1], "self") == 0) {
		sprintf(buf, "/proc/self/pagemap");
	} else {
		sprintf(buf, "/proc/%d/pagemap", atoi(argv[1]));
	}
	int fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("Cannot oper pagemap");
		return -1;
	}
	PagemapEntry entry;
	if (pagemap_get_entry(&entry, fd, atol(argv[2])) < 0) {
		fprintf(stderr, "Something went wrong");
		return -1;
	}
	printf("%d", entry->present);

	return 0;
}

