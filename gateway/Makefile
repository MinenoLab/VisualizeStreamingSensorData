CXX     = g++
CXXFLAGS= -c  -lt -g
LD      = g++
LDFLAGS = -g -o $(PROGRAM)
SOURCES = mygettimeofday.cpp gatewaymain.cpp udp_listen.cpp ringbuffer.cpp create_thread_for_client.cpp WS_accept_thread.cpp tcp_listen.cpp client_manager.cpp ws_handshake.cpp send_data_thread.cpp send_to_client_thread.cpp my_json_encode.cpp get_max_sock.cpp set_fds.cpp recv_frame_from_client.cpp recv_frame_from_client_thread.cpp
 
LIBSOURCES = ./cwebsocket/lib/websocket.c
PROGRAM = gatewaymain
OBJS	= $(SOURCES:.cpp=.o)
LIBOBJS = $(LIBSOURCES:.c=.o)
LIBS	= -lpthread -lm

.cpp.o:
	$(CXX) $(CXXFLAGS) $<

$(PROGRAM): $(OBJS) $(LIBOBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBOBJS) $(LIBS)

clean: ;
	rm $(OBJS) $(LIBOBJS) $(PROGRAM)


