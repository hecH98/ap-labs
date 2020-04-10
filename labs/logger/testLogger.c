int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main() {
	infof("Standard\n");
	warnf("Warning?\n");
	errorf("Error\n");
	panicf("Panic!\n");
	return 0;
}