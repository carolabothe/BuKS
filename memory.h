#ifindef MEMORY_H		//if not defined –> header wird nur einmal eingezogen!
	#define MEMORY_H
	#define MEM_SIZE 10240
	void memory_init();
	void* memory_allocate(int32_t byte_count);
	void memory_free(void* pointer);
	void memory_print();
#endif