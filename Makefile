KOKKOS_PATH = ${HOME}/kokkos
KOKKOS_DEVICES = "Cuda"
EXE_NAME = "mandelbrot"

SRC = Mandelbrot.cpp

default: build

LINKFLAGS = 

ifneq (,$(findstring Cuda,$(KOKKOS_DEVICES)))
CXX = ${KOKKOS_PATH}/config/nvcc_wrapper
CXXFLAGS = -ccbin mpicxx
LINKFLAGS = -ccbin mpicxx
EXE = ${EXE_NAME}.cuda
KOKKOS_ARCH = "Power8,Pascal60"
KOKKOS_CUDA_OPTIONS = "enable_lambda"
else
CXX = mpicxx
EXE = ${EXE_NAME}.host
KOKKOS_ARCH = "Power"
endif

CXXFLAGS += -O3
LINK = ${CXX}

DEPFLAGS = -M

OBJ = $(SRC:.cpp=.o)
LIB =

include $(KOKKOS_PATH)/Makefile.kokkos

build: $(EXE)

$(EXE): $(OBJ) $(KOKKOS_LINK_DEPENDS)
	$(LINK) $(KOKKOS_LDFLAGS) $(LINKFLAGS) $(EXTRA_PATH) $(OBJ) $(KOKKOS_LIBS) $(LIB) -o $(EXE)

clean: kokkos-clean
	rm -f *.o *.cuda *.host

# Compilation rules

%.o:%.cpp $(KOKKOS_CPP_DEPENDS)
	$(CXX) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) $(CXXFLAGS) $(EXTRA_INC) -c $<
