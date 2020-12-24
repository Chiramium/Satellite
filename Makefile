#
# Makefile
#

# 最終目的のファイル
TARGET = Satellite.exe

# ソースファイルの一覧
SRCS = Satellite.c

# オブジェクトファイルの一覧
OBJS = ${SRCS:.c=.o}

# ヘッダファイルの一覧
HEADERS =

# コンパイラ・リンカの指定
CC = gcc
CCFLAGS = -Wall -I/usr/include/opengl
LD = gcc
LDFLAGS =
LIBS = -lglpng -lglut32 -lglu32 -lopengl32

#OBJSからTARGETを作る方法
$(TARGET) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

# *.c から *.o を作る方法
.c.o :
	$(CC) $(CCFLAGS) -c $<

# *.oは HEADERS と MakeFile に依存
$(OBJS) : $(HEADERS) Makefile

# make cleanとしたときに実行されるコマンド
clean :
	rm -f $(TARGET) $(OBJS) core *-