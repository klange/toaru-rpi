extern void Print(char *, int len);
extern void UsbInitialise();
extern int  ReadLine(char * buf, int len);
extern int  kprintf(const char *fmt, ...);

unsigned int strlen(const char *str) {
	int i = 0;
	while (str[i] != (char)0) {
		++i;
	}
	return i;
}

void print(char * str) {
	Print(str, strlen(str));
}

int main() {
	UsbInitialise();

	kprintf("Hello world! %d\n", 42);

	while (1) {
		char buf[256];
		ReadLine(buf, 256);
		kprintf("You typed: %s\n", buf);
	}
}
