PROGECT_NAME = CPP4_3DVIEWER_V2.0
BUILD_DIR = build 
DIST_DIR = $(PROGECT_NAME)_dist
DOC_DIR = docs
.PHONY: all install uninstall clean dvi dist tests format valgrind_tests

all: install

install:
	mkdir -p $(BUILD_DIR)
	cmake -B $(BUILD_DIR) -DENABLE_TEST_COVERAGE=ON
	cd $(BUILD_DIR) && make
	@echo "программа собрана и скомпилированна"

gui: install
	cd build/gui && ./gui

uninstall: clean

clean:
	rm -rf $(BUILD_DIR) coverage.info coverage_report model_coverage.info $(DOC_DIR)
	@echo "отчиска проведена"

dvi:
	mkdir -p $(DOC_DIR)
	doxygen Doxyfile
	@echo "Документация сгенерирована в папке $(DOC_DIR)"

dist:
	mkdir -p $(DIST_DIR)
	cp -r ./Makefile ./tests ./gui ./model ./commons ./Doxyfile ./CMakeLists.txt $(DIST_DIR)
	tar -czvf $(DIST_DIR).tar.gz $(DIST_DIR)/
	rm -rf $(DIST_DIR)
	@echo "Архив ../$(DIST_DIR).tar.gz создан"

tests:
	cd $(BUILD_DIR) && ctest

gcov_tests: tests
	# Генерация отчета покрытия
	lcov --capture --directory $(BUILD_DIR) --output-file coverage.info
	# Фильтрация покрытия только для src/model
	lcov --extract coverage.info "*/src/model/*" --output-file model_coverage.info
	# Генерация HTML-отчета
	genhtml model_coverage.info --output-directory coverage_report
	@echo "Отчет покрытия сгенерирован в папке coverage_report"

format:
	clang-format -style=Google -i commons/*.hh
	clang-format -style=Google -i gui/*.hh gui/*.h gui/*.cpp
	clang-format -style=Google -i model/*.hh model/*.cpp
	clang-format -style=Google -i tests/*.cpp

valgrind_tests:
	cd $(BUILD_DIR) && valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./tests/s21_transform_tests
	cd $(BUILD_DIR) && valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./tests/testParser
	@echo "Тесты запущены с использованием Valgrind"
