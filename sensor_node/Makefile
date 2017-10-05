CC      = gcc 			#Cコンパイルコマンド
CFLAGS  = -pg -c -Wall -lt -g 	#Cコンパイルオプション
LD      = gcc			
LDFLAGS = -pg -g -o $(PROGRAM)	#LD呼び出し時のオプション
SOURCES = sensor_io_handle.c nodemain.c mygettimeofday.c udp_setup.c #ソースファイルの指定
PROGRAM = nodemain		#生成ファイル名?
OBJS	= $(SOURCES:.c=.o)
LIBS	= -lpthread -lm 

.c.o:
	$(CC) $(CFLAGS) $<

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS)

clean: ;
	rm $(OBJS) $(PROGRAM) gmon.out


