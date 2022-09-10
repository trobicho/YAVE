CC = g++
GLS = glslangValidator
CXXFLAGS	=	-std=c++14 -g

NAME = yave

INCLUDE_PATH = /home/tom/projects/lib
SRCS_PATH	=	./src
HDRS_PATH	=	./src
OBJS_PATH	=	./obj
SHADERS_PATH	=	./shaders

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	INCS_FLAGS = -I$(VULKAN_SDK)/include \
		-I$(GLFW3_PATH)/include \
		-I $(INCLUDE_PATH)
	LDFLAGS = -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan -lm
else
	LDFLAGS = -L$(GLFW3_PATH)/lib -L$(VULKAN_SDK)/lib `pkg-config --static --libs glm` -lvulkan -lglfw -lm
endif

SHADERS_NAME =	fractal.frag \
                shader.vert \

SHADERS_RESULT_NAME =	frag.spv \
                        vert.spv \

SRCS_NAME =	main.cpp \
			AppTest.cpp \
			core/devicePickerBasic.cpp \
			core/YaveParamsValidator.cpp \
			core/YaveRenderer.cpp \
			core/YaveRendererHelper.cpp \
			core/YaveRenderProgs.cpp \
			core/YaveContext.cpp \
			handlers/YaveBasicHandler.cpp \
			YaveLib/YaveUtils.cpp \
			test/pipelineCreationTest.cpp

HDRS_NAME =	AppTest.h \
			core/YAVE.h \
			core/devicePickerBasic.h \
			core/YaveParams.h \
			core/YaveContext.h \
			core/YaveContextGlobal.h \
			core/YaveRenderer.h \
			core/YaveRenderProgs.h \
			core/YaveRendererHelper.h \
			handlers/YaveBasicHandler.h \
			YaveLib/YaveException.h \
			YaveLib/YaveUtils.h \
			test/pipelineCreationTest.h

OBJS_NAME	=	$(SRCS_NAME:.cpp=.o) 

SRCS = $(addprefix $(SRCS_PATH)/, $(SRCS_NAME))
HDRS = $(addprefix $(HDRS_PATH)/, $(HDRS_NAME))
OBJS = $(addprefix $(OBJS_PATH)/, $(OBJS_NAME))
SHADERS = $(addprefix $(SHADERS_PATH)/, $(SHADERS_NAME))
SHADERS_RESULT = $(addprefix $(SHADERS_PATH)/, $(SHADERS_RESULT_NAME))

all: $(NAME) Makefile

$(NAME): $(SRCS) $(HDRS) $(OBJS) Makefile
	$(CC) $(CXXFLAGS) $(INCS_FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.cpp $(HDRS) Makefile
	@test -d $(OBJS_PATH) || mkdir -pm 775 $(OBJS_PATH)
	@test -d $(@D) || mkdir -pm 775 $(@D)
	@echo "\033[38;2;0;255;0m[cc]\033[0m: $< -> $@"
	@printf "\e[1A"
	@$(CC) $(CXXFLAGS) -I $(HDRS_PATH) $(INCS_FLAGS) -c $< -o $@
	@printf "\e[0K"

shaders: $(SHADERS_RESULT) Makefile

$(SHADERS_RESULT): $(SHADERS)
	$(GLS) -V $(SHADERS)
	mv *.spv $(SHADERS_PATH)/

clean:
	rm -f $(OBJS)
	rm -f $(SHADERS_PATH)/*.spv

fclean: clean
	rm -f $(NAME)

re: fclean all
