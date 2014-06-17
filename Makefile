# DataIOのためのMakefile
CXX  = g++
SRCS  = DataIO.cpp
# OBJS = DataIO.o
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(SRCS:%.cpp=%.d)

# FLAGS = -Wc++11-extensions
FLAGS = -std=c++11

# boostへのパス
# 環境に合わせて変更してください
BOOST_INCPASS = -I/opt/local/include

# ---- テスト用の設定 ----

# google test関連
# 環境に合わせて変更してください
GTEST_INCPASS    = -I/usr/local/include/
GTEST_LIBPASS    = -L/usr/local/lib/gtest/
GTEST_LIBS       = -lgtest -lpthread

TEST_DIR = test
TEST_SRCS = $(TEST_DIR)/testDataIO.cpp
TEST_OBJS    = $(TEST_SRCS:%.cpp=%.o)
TEST_PROGRAM = $(TEST_DIR)/testDataIO.test

all:            $(OBJS)

clean:;         rm -f *.o *~ test/*.o test/*.test *.d

.cpp.o:
	$(CXX) -c -MMD $(FLAGS) $< 

$(TEST_OBJS):	$(TEST_SRCS)
	$(CXX) $(FLAGS) -c -o $@ $^ $(GTEST_INCPASS)

test:		$(TEST_OBJS) $(OBJS)
	$(CXX) $(FLAGS) $(TEST_OBJS) $(OBJS) -o $(TEST_PROGRAM) $(GTEST_LIBPASS) $(GTEST_LIBS)

-include $(DEPS)
