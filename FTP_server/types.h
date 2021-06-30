#define MAXNAME 256
#define MAXDATA 256


typedef struct response sizeIfExist;
struct response {
off_t st_size;
int exist;
};

typedef struct buffer data;
struct buffer {
	ssize_t taille;
	char donnee [MAXDATA];
};
typedef struct connx idconnx;
struct connx{
char ipAddress[16];
int port;
};
