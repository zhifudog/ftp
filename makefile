TARGETS		= ./myftp
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

CPPFLAGS = -g 
$(TARGETS):$(OBJS)
	@echo "creating excutable -*- $@ -*- ..."
	$(CXX) $(CPPFLAG) -o $@  $(OBJS) 
	@echo "Compile end ......"
					                
clean:
	@echo "Removing linked and compiled files ..."
	rm -f $(OBJS) $(TARGETS)
