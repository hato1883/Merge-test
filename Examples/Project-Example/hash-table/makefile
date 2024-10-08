
###
# Constant delerations
###

# Directory structure
DEPDIR			:=dep
BINDIR			:=bin
OBJDIR			:=obj
SRCDIR			:=src
TESTDIR			:=test
COVDIR			:=coverage
PROFDIR			:=profiling

# C Compiler
CC     				:=gcc
# Compiler Flags
CFLAGS 				:=-Wall -Wextra -pedantic -g
# Compiler Linker Flags
LDFLAGS 			:=-lm
# C Unit Dependancy / link
CUNIT_LINK     		:=-lcunit
# Compiler Coverage Flags
CCOVFLAGS 			:=-O0 --coverage
# Compiler Profiling Flags
CPROFFLAGS 			:=-O3 -pg
# Error testing linking flags
ERRORFLAGS 			:=-Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=free
SRC_COV_PERC_PASS 	:=100
SRC_COV_PERC_WARN 	:=90


# Memory leak Test runner
MEMLEAK_RUNNER		:= valgrind
MEMLEAK_PRE_FLAGS	:= --leak-check=full --error-exitcode=103 -s
MEMLEAK_POST_FLAGS	:= > /dev/null

# Heap Profiling
HEAP_RUNNER			:= valgrind
HEAP_PRE_FLAGS		:= --tool=massif --heap=yes --stacks=yes
HEAP_POST_FLAGS		:= > /dev/null 2>&1

# Cache Profiling
CACHE_RUNNER		:= valgrind
CACHE_PRE_FLAGS		:= --tool=cachegrind
CACHE_POST_FLAGS	:= > /dev/null 2>&1

# Find all .c files in our project
# Both in test/ and src/
EXES				:= linked_list_test linked_list_error_handeling_test iterator_test iterator_error_handeling_test hash_table_test hash_table_error_handeling_test hash_table_exe freq_count_exe
SRCS				:= ${filter-out ${addsuffix .c,$(EXES)}, ${shell find ./ -name '*.c' | xargs -n1 basename}}
DEPS				:= ${addprefix $(DEPDIR)/,${SRCS:.c=.d}} ${addprefix $(DEPDIR)/,${EXES:=.d}}
OBJS				:= ${addprefix $(OBJDIR)/,${filter-out oom.o,${SRCS:.c=.o}}}

# Make sure that any objects and dependecies that are created do not get deleted after target is run.
.PRECIOUS: $(OBJDIR)/%.o $(DEPDIR)/%.d

default: ${addprefix $(BINDIR)/,freq_count_exe}
	./bin/freq_count_exe src/text_files/bible.txt

build: ${addprefix $(BINDIR)/,$(EXES)}

clean:
	@rm -rf ./$(BINDIR)/ ./$(OBJDIR)/ ./$(DEPDIR)/ ./$(COVDIR)/
	@rm -f gmon.out gmon.sum

deepclean: clean
	@rm -rf ./$(PROFDIR)/ ./logs/

installdirs: | $(COVDIR)/ $(BINDIR)/ $(OBJDIR)/ $(DEPDIR)/

all: ${addprefix $(BINDIR)/,$(EXES)}
	@echo "Executables found: ${EXES}"
	@echo "Sources found: ${SRCS}"
	@echo "Objects found: ${OBJS}"
	@echo "Dependencies found: ${DEPS}"


.PHONY: test
test: unit_test mem_test

#######################################################
#                                                     #
#      Target to create colored unit test report      #
#                                                     #
#######################################################

# All files to execute to test code coverage
# By default this will be all files ending in _test.c
TEST_MANUAL		:= ${shell find ./ -name '*_test.c'}

# DO NOT EDIT VARIABLE BELLOW
# add files in MANUAL variable above
TEST_BINARIES	:= ${addprefix $(BINDIR)/,${basename ${shell echo $(TEST_MANUAL) | xargs -n1 basename}}}

.PHONY: unit_test
.ONESHELL:
unit_test: $(TEST_BINARIES)
# Escape first @ sign when using oneshell with comment
	@mkdir -p logs/
	@echo ""
#	Set default exit code to 0
	SHOULD_PASS=0
	@echo ""
	@echo "Running tests"
	@echo ""

	for v in $(TEST_BINARIES); do \
		NAME=$$( echo $$v | xargs -n1 basename)

		./$${v} > logs/$${NAME}_log.txt 2>/dev/null

		cat logs/$${NAME}_log.txt | \
			awk 'BEGIN {
				err = 0;
				is_at_summery = 0;
				cmd="cat logs/$${NAME}_log.txt | grep -oP \"\\s*asserts\\s+([0-9]+\\s+){3}\\K[0-9]+\""
				cmd | getline err
				close(cmd)
				err = err+0;
				} {
				if ($$0 ~ /Suite:.*/) {
					print $$0;
				}
				
				if (err > 0) {
					if ($$0 ~ /.*passed/) {
						print "\033[2;30m" $$0 "\033[0;37m";
					} else if ($$0 ~ /.*failed/) {
						print "\033[1;31m" $$0 "\033[0;37m";
					} else {
					};
				} else {
					if ($$0 ~ /.*passed/) {
					} else if ($$0 ~ /.*failed/) {
					} else {
					};
				}

				if ($$0 ~ /.*Run Summary.*/) {
					is_at_summery = 1;
				}

				if (is_at_summery == 1) {
					if (err > 0) {
						print "\033[1;31m" $$0 "\033[0;37m";
					} else {
						print $$0;
					}
				}
				
			} END {
				exit err;
			}'
		EXIT_CODE=$$?

		if [ $$EXIT_CODE -ne 0 ]; then
#			Update SHOULD_PASS variable to inform of crash
			SHOULD_PASS=1; \
		else
#			Remove temporary log file, user will not read it anyways
			rm -f logs/$${NAME}_log.txt
#			Inform of of passing test
			echo "\033[2;32mUnit test Passed for $$v" "\033[0;37m"; \
		fi

		echo ""; \
    done
	

	if [ $$SHOULD_PASS -ne 0 ]; then
#			Update SHOULD_PASS variable to inform of crash
		echo "\033[1;31m###" "\033[0;37m"; \
		echo "\033[1;31m# One or more Unit Test Suites has failed!" "\033[0;37m"; \
		echo "\033[1;31m###" "\033[0;37m"; \
	else
		rm -rf logs/
#			Inform of of passing test
		echo "\033[1;32m###" "\033[0;37m"
		echo "\033[1;32m# All Test Suites passed all thier tests" "\033[0;37m"
		echo "\033[1;32m###" "\033[0;37m"; \
	fi
	@echo ""

#	If one of source or test coverage fial then exit make command with non 0 exit code
	if [ $$SHOULD_PASS -eq 1 ]; then
		exit 2; \
	fi
#


############################################
#                                          #
#      Target to create memory report      #
#                                          #
############################################

.PHONY: mem_test
.ONESHELL:
mem_test: ${TEST_BINARIES}
	@echo
	HAS_LEAK=0
	for v in $(TEST_BINARIES) ; do \
#		Fancy printing of status
		echo "#" "\033[1;30m" "Running Memory Leak test using valgrind on:" "\033[0;37m"
		echo "#" "\033[1;30m" "    $${v}..." "\033[0;37m"
#		Run Memory leak testing program
		$(MEMLEAK_RUNNER) $(MEMLEAK_PRE_FLAGS) $$v $(MEMLEAK_POST_FLAGS) 2>log.txt
#		Capture exit code
		EXIT_CODE=$$?
		if [ $$EXIT_CODE -ne 0 ]; then
#			Memory leak found Set output to bold red
			echo "\033[2;31m"
#			Print log of found leak
			cat log.txt
#			Reset output to Normal White
			echo "\033[0;37m"
#			Inform of error with message
			echo "#" "\033[2;31m" "ERROR: $$v has ecounterd Memory Leaks" "\033[0;37m"
#			Update HAS_LEAK variable to inform of crash
			HAS_LEAK=1; \
		else
#			Remove temporary log file, user will not read it anyways
			rm -f ./log.txt
#			Inform of of passing test
			echo "#" "\033[2;32m" "Memory Leak Passed for $$v" "\033[0;37m"; \
		fi
		echo ; \
    done
#	If HAS_LEAK is set to 1, exit with non 0 code
	if [ $$HAS_LEAK -eq 1 ]; then
		exit $$HAS_LEAK
	fi
#






#######################################################
#                                                     #
# Target to create colored coverage report (and html) #
#                                                     #
#######################################################

# All files to execute to test code coverage
# By default this will be all files ending in _test.c
COVERAGE_MANUAL		:= ${shell find ./ -name '*_test.c'}

# DO NOT EDIT VARIABLE BELLOW
# add files in MANUAL variable above
COVERAGE_BINARIES	:= ${addprefix $(BINDIR)/,${basename ${shell echo $(COVERAGE_MANUAL) | xargs -n1 basename}}}

.PHONY: coverage
.ONESHELL:
coverage: $(COVERAGE_BINARIES) | $(COVDIR)/
# Escape first @ sign when using oneshell with comment
	@echo ""
#	Set default exit code to 0
	SHOULD_PASS=0
	@echo ""
	@echo "Getting Coverage..."
	@echo ""

#	Reset previous coverage tracking if such a thing exists
	lcov --directory $(OBJDIR)/ --zerocounters -q

#	Run each coverage contributing file once
#	$(foreach var,$(COVERAGE_BINARIES),./$(var) > /dev/null;)
	for v in $(COVERAGE_BINARIES); do \
		echo "Collecting Coverage from $${v}" ; \
		./$${v} > /dev/null ; \
    done

	@echo ""
	@echo "Generating report..."
	@echo ""
#	Generate lcov info file from data
	lcov -q --base-directory . --directory $(OBJDIR)/ -c -o $(COVDIR)/inlupp1.info  2> /dev/null
#	Generate html from lcov file
	genhtml -q -o $(COVDIR)/inlupp1 -t "inlupp1 test coverage" --num-spaces 4 $(COVDIR)/inlupp1.info

#	Generate report summary to report in terminal
	TEST_COVERAGE=$$(gcovr -r $(TESTDIR)/ $(OBJDIR)/ -e $(TESTDIR)/oom.c --txt  2> /dev/null)

#	Print colored version of test files coverage output
	echo "$$TEST_COVERAGE" | \
	awk 'BEGIN {err = 0} {
		if ($$1 ~ /.*\.c/) {
			if(NR > 6 && $$2 - $$3 <= 5) {
				print "\033[2;32m" $$0 "\033[0;37m";
			} else if($$4+0 >= 90) {
				print "\033[33m" $$0 "\033[0;37m";
				err = err < 1 ? 1 : err
			} else {
				print "\033[1;31m" $$0 "\033[0;37m";
				err = 2
			}
		} else if ($$1 ~ /TOTAL/) {
			print "\033[1;30m""Note: The 5 uncoverd lines on each file is due to test suite crash handeling""\033[0;37m";
			print "------------------------------------------------------------------------------";
			if(($$2 - $$3) / (NR-8) <= 5) {
				print "\033[1;32m" $$0 "\033[0;37m";
			} else if($$4+0 >= 90) {
				print "\033[1;33m" $$0 "\033[0;37m";
			} else {
				print "\033[1;31m" $$0 "\033[0;37m";
			}
		} else {
			print $$0;
		}
	} END {
		exit err;
	}'
	TEST_STATUS=$$?
#	Print CI status
#	Print information regarding if coverage of cunit test files are good or not
	if [ $$TEST_STATUS -eq 1 ]; then
		echo "\033[1;33m""Warrning: Test contains small amount unused code, Marking as Passed" "\033[0;37m"
	elif [ $$TEST_STATUS -eq 2 ]; then
		echo "\033[1;31m""###""\033[0;37m"
		echo "\033[1;31m""# ERROR: One or more test contains LARGE amount unused code. Marking as failed" "\033[0;37m"
		echo "\033[1;31m""###""\033[0;37m"
		SHOULD_PASS=1; \
	fi
	@echo ""

	SRC_COVERAGE=$$(gcovr -r $(SRCDIR)/ $(OBJDIR)/ --txt 2> /dev/null)
#	Print colored version of src files coverage output
	echo "$$SRC_COVERAGE" | \
	awk 'BEGIN {lowest_coverage = 100}{
		if ($$1 ~ /.*\.c/) {
			if(NR > 6 && $$4+0 >= $(SRC_COV_PERC_PASS)) {
				print "\033[2;32m" $$0 "\033[0;37m";
				lowest_coverage = $$4+0 < lowest_coverage ? $$4+0 : lowest_coverage
			} else if($$4+0 >= $(SRC_COV_PERC_WARN)) {
				print "\033[33m" $$0 "\033[0;37m";
				lowest_coverage = $$4+0 < lowest_coverage ? $$4+0 : lowest_coverage
			} else {
				print "\033[1;31m" $$0 "\033[0;37m";
				lowest_coverage = $$4+0 < lowest_coverage ? $$4+0 : lowest_coverage
			}
		} else if ($$1 ~ /TOTAL/) {
			if($$4+0 >= $(SRC_COV_PERC_PASS)) {
				print "\033[1;32m" $$0 "\033[0;37m";
			} else if($$4+0 >= $(SRC_COV_PERC_WARN)) {
				print "\033[1;33m" $$0 "\033[0;37m";
			} else {
				print "\033[1;31m" $$0 "\033[0;37m";
			}
		} else {
			print $$0;
		}
	} END {
		exit lowest_coverage;
	}'
	SRC_PERCENT=$$?

#	Print information regarding if coverage of source files are good or not
	if [ $$SRC_PERCENT -lt $(SRC_COV_PERC_PASS) ] && [ $$SRC_PERCENT -ge $(SRC_COV_PERC_WARN) ]; then
		echo "\033[1;33m""###""\033[0;37m"
		echo "\033[1;33m""# Warrning: One or more source code files contains small amount unused code, Marking as Passed" "\033[0;37m"
		echo "\033[1;33m""###""\033[0;37m"
	elif [ $$SRC_PERCENT -lt $(SRC_COV_PERC_WARN) ]; then
		echo "\033[1;31m""###""\033[0;37m"
		echo "\033[1;31m""# ERROR: One or more source code files contains LARGE amount unused code. Marking as failed" "\033[0;37m"
		echo "\033[1;31m""###""\033[0;37m"
		SHOULD_PASS=1; \
	fi

	@echo ""
	@echo "Code Coverage Analysis Completed"
	@echo ""

#	If one of source or test coverage fial then exit make command with non 0 exit code
	if [ $$SHOULD_PASS -eq 1 ]; then
		exit 2; \
	fi
#


##################################################
#                                                #
# Target to run profiling tools on choosen files #
#                                                #
##################################################

# All files to execute to generate a profile of program
PROFILE_MANUAL			:= freq_count_exe_profiling

# DO NOT EDIT VARIABLE BELLOW
# add files in MANUAL variable above
PROFILE_BINARIES		:= ${addprefix $(BINDIR)/,${basename ${shell echo $(PROFILE_MANUAL) | xargs -n1 basename}}}
PROFILE_BIN_ARGUMENTS	:= src/text_files/bible.txt

.PHONY: profile
.ONESHELL:
profile: $(PROFILE_BINARIES) $(patsubst %_profiling,%,$(PROFILE_BINARIES)) | $(PROFDIR)/ 
# Escape first @ sign when using oneshell with comment
	@echo ""
	@echo ""
	@echo "Collecting runtime for all given files"
	@echo ""

#	Save short commit sha in variable, used for naming current commits profiling results.
	GIT_COMMIT_ID=$$(git rev-parse --short HEAD)
	@mkdir -p $(PROFDIR)/$${GIT_COMMIT_ID}/
	@mkdir -p $(PROFDIR)/$${GIT_COMMIT_ID}/data

#	Run each profiling contributing file once
	for v in $(PROFILE_BINARIES); do \
#		Get filname without path
		NAME=$$( echo $$v | xargs -n1 basename)
#		Get normal compilation of file without -pg flag
		NON_PG=$$( echo "$${v}" | grep -o '.*exe' )
		echo "NOT PROFILING binary: $${NON_PG}"
#		Set how many times to run a file to reduce jitter in results
		count=20
#		For each time create a file with format profiling/<executable-file-name>_result_index
#		where index is loop step that is left zeropaded to always be 3 digits long
		echo "Collecting Runtime profile for $$v"
		for i in $$(seq $$count); do \
			echo "Executing: $${v}, Remaining executions: $${i}/$${count}..."
			INDEX=$$(printf %03d $${i})
#			run time command with verbose flag to get all info and save in file mentioned above
			RESULT=$$( ( ( command time -v $$v ${PROFILE_BIN_ARGUMENTS}; ) 1>/dev/null; ) 2>$(PROFDIR)/$${GIT_COMMIT_ID}/$${NAME}_result_$${INDEX}; )

#			if it is the first loop then move gmon.out to gmon.sum
			if [ $$i -eq 1 ]; then
				mv gmon.out gmon.sum
			else
#				Otherwise merge new gmon.out into gmon.sum
				gprof -s $$v gmon.out gmon.sum
			fi; \
		done
		echo ""

#		Save all temporary result files into a variable
		TMP_RESULT_FILES=$$(find ./ -name "$${NAME}_result*")
#		for each file check the secound line
#		if User time (seconds): contains a lower number than we have stored
#		update our runtime var and set file by dividing global line nummer by 23 due to each file being 23 lines long and casting to int
#		when we reach the end we print the id of the file with best User time (seconds): result.
		BEST_TIME=$$(echo $${TMP_RESULT_FILES} | xargs awk 'BEGIN{runtime = -1; file = 1;}{
			if (FNR == 2) {
				if (runtime == -1) {
					runtime = $$4+0;
				} else if (runtime > $$4+0) {
					runtime = $$4+0;
					file = int(NR / 23) + 1;
				}
			}
		} END {print sprintf("%03d", file);}')

#		rename the temporary file with best result into <Commit-sha>_<executable-name>_runtime
		mv $(PROFDIR)/$${GIT_COMMIT_ID}/$${NAME}_result_$${BEST_TIME} $(PROFDIR)/$${GIT_COMMIT_ID}/$${NAME}_runtime
#		Remove all other temporary files as they will not be needed
		rm -rf $${TMP_RESULT_FILES}
#		create report using the gmon.sum and save into <Commit-sha>_<executable-name>_function_usage_analysis
		gprof -b $$v gmon.sum > $(PROFDIR)/$${GIT_COMMIT_ID}/$${NAME}_function_usage_analysis


#		Get Heap profile		
		echo ""
		echo "Getting heap profile from $${NON_PG}"
		$(HEAP_RUNNER) $(HEAP_PRE_FLAGS) $${NON_PG} ${PROFILE_BIN_ARGUMENTS} $(HEAP_POST_FLAGS)
		HEAP_OUTPUT=$$(find . -name "massif.out.*" | tail -1 | grep -o -e "[[:digit:]]\{1,\}")
		mv massif.out.$${HEAP_OUTPUT} $(PROFDIR)/$${GIT_COMMIT_ID}/data/$${NAME}_massif.data
		ms_print "$(PROFDIR)/$${GIT_COMMIT_ID}/data/$${NAME}_massif.data"  > $(PROFDIR)/$${GIT_COMMIT_ID}/$${NAME}_heap_profile
		echo "Heap profile extracted from $${NON_PG}"
		echo ""
		

#		Get Cache profile
		echo ""
		echo "Getting cache profile from $${NON_PG}"
		$(CACHE_RUNNER) $(CACHE_PRE_FLAGS) $${NON_PG} ${PROFILE_BIN_ARGUMENTS} $(CACHE_POST_FLAGS)
		CACHE_OUTPUT=$$(find . -name "cachegrind.out.*" | tail -1 | grep -o -e "[[:digit:]]\{1,\}")
		mv cachegrind.out.$${CACHE_OUTPUT} $(PROFDIR)/$${GIT_COMMIT_ID}/data/$${NAME}_cachegrind.data
		cg_annotate "$(PROFDIR)/$${GIT_COMMIT_ID}/data/$${NAME}_cachegrind.data" > $(PROFDIR)/$${GIT_COMMIT_ID}/$${NAME}_cache_profile
		echo "Cache profile extracted from $${NON_PG}"
		echo ""; \
    done

	@echo "###"
	@echo "# Code Profiling Completed"
	@echo "# "
	@echo "# Results can be found in $(PROFDIR)/$${GIT_COMMIT_ID}/"
	@echo "###"

# Build all dependencies

###
# Executable rule's
# Specific rules where nececary
###

# Common Objects used when testing hash table
HASH_TEST_COMMON_OBJS		:= $(OBJDIR)/hash_table.coverage.o $(OBJDIR)/linked_list.coverage.o $(OBJDIR)/iterator.coverage.o  | $(BINDIR)/

$(BINDIR)/hash_table_test: $(OBJDIR)/hash_table_test.coverage.o $(HASH_TEST_COMMON_OBJS)
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $^ $(CUNIT_LINK)


$(BINDIR)/hash_table_error_handeling_test: $(OBJDIR)/hash_table_error_handeling_test.coverage.o $(OBJDIR)/oom.coverage.o $(HASH_TEST_COMMON_OBJS)
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $(ERRORFLAGS) $^ $(CUNIT_LINK)



# Common Objects used when testing linked list
LIST_TEST_COMMON_OBJS		:= $(OBJDIR)/linked_list.coverage.o $(OBJDIR)/iterator.coverage.o  | $(BINDIR)/

$(BINDIR)/linked_list_test: $(OBJDIR)/linked_list_test.coverage.o $(LIST_TEST_COMMON_OBJS)
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $^ $(CUNIT_LINK)


$(BINDIR)/linked_list_error_handeling_test: $(OBJDIR)/linked_list_error_handeling_test.coverage.o $(OBJDIR)/oom.coverage.o $(LIST_TEST_COMMON_OBJS)
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $(ERRORFLAGS) $^ $(CUNIT_LINK)



# Common Objects used when testing iterator
ITERATOR_TEST_COMMON_OBJS	:= $(OBJDIR)/iterator.coverage.o  $(OBJDIR)/linked_list.coverage.o | $(BINDIR)/

$(BINDIR)/iterator_test: $(OBJDIR)/iterator_test.coverage.o $(ITERATOR_TEST_COMMON_OBJS)
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $^ $(CUNIT_LINK)


$(BINDIR)/iterator_error_handeling_test: $(OBJDIR)/iterator_error_handeling_test.coverage.o $(OBJDIR)/oom.coverage.o $(ITERATOR_TEST_COMMON_OBJS)
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $(ERRORFLAGS) $^ $(CUNIT_LINK)



$(BINDIR)/hash_table_exe: $(OBJDIR)/hash_table_exe.o $(OBJDIR)/hash_table.o $(OBJDIR)/linked_list.o $(OBJDIR)/iterator.o | $(BINDIR)/
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $^

$(BINDIR)/hash_table_exe_profiling: $(OBJDIR)/hash_table_exe.profiling.o $(OBJDIR)/hash_table.profiling.o $(OBJDIR)/linked_list.profiling.o $(OBJDIR)/iterator.profiling.o | $(BINDIR)/
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CPROFFLAGS) $^


$(BINDIR)/freq_count_exe: $(OBJDIR)/freq_count_exe.o $(OBJDIR)/hash_table.o $(OBJDIR)/linked_list.o $(OBJDIR)/iterator.o | $(BINDIR)/
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $^

$(BINDIR)/freq_count_exe_profiling: $(OBJDIR)/freq_count_exe.profiling.o $(OBJDIR)/hash_table.profiling.o $(OBJDIR)/linked_list.profiling.o $(OBJDIR)/iterator.profiling.o | $(BINDIR)/
	@echo "Linking: $@"
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CPROFFLAGS) $^


###
# Fallback rules
###
$(BINDIR)/%_exe: $(OBJS) $(OBJDIR)/%_exe.o | $(BINDIR)/
	@echo "No rule to make target: $@, Using default..."
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $^

$(BINDIR)/%_error_handeling_test: $(OBJS) $(OBJDIR)/%_error_handeling_test.o $(OBJDIR)/oom.o | $(BINDIR)/
	@echo "No rule to make target: $@, Using default..."
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $(ERRORFLAGS) $^ $(CUNIT_LINK)

$(BINDIR)/%_test: $(OBJS) $(OBJDIR)/%_test.o | $(BINDIR)/
	@echo "No rule to make target: $@, Using default..."
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $^ $(CUNIT_LINK)

###
# Wildcard rules
###


###
# Profiling object files
###
$(OBJDIR)/%.profiling.o: $(SRCDIR)/%.c | $(OBJDIR)/ $(DEPDIR)/
	@echo "Compiling: $@"
	$(CC) -o $@ -c $(CFLAGS) $(CPROFFLAGS) $<
	${CC} -MM -MP $< -MT "$@" -o $(DEPDIR)/${shell basename ${basename $@}}.d
	@sed "s/src\/..\///g" $(DEPDIR)/${shell basename ${basename $@}}.d  > $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@mv $(DEPDIR)/${shell basename ${basename $@}}.d $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@touch $@

$(OBJDIR)/%.profiling.o: $(TESTDIR)/%.c | $(OBJDIR)/ $(DEPDIR)/
	$(CC) -o $@ -c $(CFLAGS) $(CPROFFLAGS) $<
	${CC} -MM -MP $< -MT "$@" -o $(DEPDIR)/${shell basename ${basename $@}}.d
	@sed "s/test\/..\///g" $(DEPDIR)/${shell basename ${basename $@}}.d  > $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@mv $(DEPDIR)/${shell basename ${basename $@}}.d $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@touch $@


###
# Coverage object files
###
$(OBJDIR)/%.coverage.o: $(SRCDIR)/%.c | $(OBJDIR)/ $(DEPDIR)/
	@echo "Compiling: $@"
	$(CC) -o $@ -c $(CFLAGS) $(CCOVFLAGS) $<
	${CC} -MM -MP $< -MT "$@" -o $(DEPDIR)/${shell basename ${basename $@}}.d
	@sed "s/src\/..\///g" $(DEPDIR)/${shell basename ${basename $@}}.d  > $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@mv $(DEPDIR)/${shell basename ${basename $@}}.d $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@touch $@

$(OBJDIR)/%.coverage.o: $(TESTDIR)/%.c | $(OBJDIR)/ $(DEPDIR)/
	@echo "Compiling: $@"
	$(CC) -o $@ -c $(CFLAGS) $(CCOVFLAGS) $<
	${CC} -MM -MP $< -MT "$@" -o $(DEPDIR)/${shell basename ${basename $@}}.d
	@sed "s/test\/..\///g" $(DEPDIR)/${shell basename ${basename $@}}.d  > $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@mv $(DEPDIR)/${shell basename ${basename $@}}.d $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@touch $@


###
# Production object files
###
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)/ $(DEPDIR)/
	@echo "Compiling: $@"
	$(CC) -o $@ -c $(CFLAGS) $<
	${CC} -MM -MP $< -MT "$@" -o $(DEPDIR)/${shell basename ${basename $@}}.d
	@sed "s/src\/..\///g" $(DEPDIR)/${shell basename ${basename $@}}.d  > $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@mv $(DEPDIR)/${shell basename ${basename $@}}.d $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@touch $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c | $(OBJDIR)/ $(DEPDIR)/
	@echo "Compiling: $@"
	$(CC) -o $@ -c $(CFLAGS) $<
	${CC} -MM -MP $< -MT "$@" -o $(DEPDIR)/${shell basename ${basename $@}}.d
	@sed "s/test\/..\///g" $(DEPDIR)/${shell basename ${basename $@}}.d  > $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@mv $(DEPDIR)/${shell basename ${basename $@}}.d $(DEPDIR)/${shell basename ${basename $@}}.new.d
	@touch $@


# Includes rules created from .c files by reading include headers
-include ${DEPS}

###
# Directory Rules
###

%/:
	mkdir -p $@