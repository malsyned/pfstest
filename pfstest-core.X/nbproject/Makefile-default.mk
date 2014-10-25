#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pfstest-core.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pfstest-core.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../core-tests-2.c ../core-tests.c ../pfstest-alloc-pic.c ../pfstest-arg-handler.c ../pfstest-arg-handlers.c ../pfstest-assert.c ../pfstest-core.c ../pfstest-list.c ../pfstest-matcher.c ../pfstest-matchers.c ../pfstest-mock.c ../pfstest-platform.c ../pfstest-value.c ../pfstest-values.c ../pic-main.c ../register-core-tests.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/core-tests-2.o ${OBJECTDIR}/_ext/1472/core-tests.o ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o ${OBJECTDIR}/_ext/1472/pfstest-assert.o ${OBJECTDIR}/_ext/1472/pfstest-core.o ${OBJECTDIR}/_ext/1472/pfstest-list.o ${OBJECTDIR}/_ext/1472/pfstest-matcher.o ${OBJECTDIR}/_ext/1472/pfstest-matchers.o ${OBJECTDIR}/_ext/1472/pfstest-mock.o ${OBJECTDIR}/_ext/1472/pfstest-platform.o ${OBJECTDIR}/_ext/1472/pfstest-value.o ${OBJECTDIR}/_ext/1472/pfstest-values.o ${OBJECTDIR}/_ext/1472/pic-main.o ${OBJECTDIR}/_ext/1472/register-core-tests.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/core-tests-2.o.d ${OBJECTDIR}/_ext/1472/core-tests.o.d ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o.d ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o.d ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o.d ${OBJECTDIR}/_ext/1472/pfstest-assert.o.d ${OBJECTDIR}/_ext/1472/pfstest-core.o.d ${OBJECTDIR}/_ext/1472/pfstest-list.o.d ${OBJECTDIR}/_ext/1472/pfstest-matcher.o.d ${OBJECTDIR}/_ext/1472/pfstest-matchers.o.d ${OBJECTDIR}/_ext/1472/pfstest-mock.o.d ${OBJECTDIR}/_ext/1472/pfstest-platform.o.d ${OBJECTDIR}/_ext/1472/pfstest-value.o.d ${OBJECTDIR}/_ext/1472/pfstest-values.o.d ${OBJECTDIR}/_ext/1472/pic-main.o.d ${OBJECTDIR}/_ext/1472/register-core-tests.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/core-tests-2.o ${OBJECTDIR}/_ext/1472/core-tests.o ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o ${OBJECTDIR}/_ext/1472/pfstest-assert.o ${OBJECTDIR}/_ext/1472/pfstest-core.o ${OBJECTDIR}/_ext/1472/pfstest-list.o ${OBJECTDIR}/_ext/1472/pfstest-matcher.o ${OBJECTDIR}/_ext/1472/pfstest-matchers.o ${OBJECTDIR}/_ext/1472/pfstest-mock.o ${OBJECTDIR}/_ext/1472/pfstest-platform.o ${OBJECTDIR}/_ext/1472/pfstest-value.o ${OBJECTDIR}/_ext/1472/pfstest-values.o ${OBJECTDIR}/_ext/1472/pic-main.o ${OBJECTDIR}/_ext/1472/register-core-tests.o

# Source Files
SOURCEFILES=../core-tests-2.c ../core-tests.c ../pfstest-alloc-pic.c ../pfstest-arg-handler.c ../pfstest-arg-handlers.c ../pfstest-assert.c ../pfstest-core.c ../pfstest-list.c ../pfstest-matcher.c ../pfstest-matchers.c ../pfstest-mock.c ../pfstest-platform.c ../pfstest-value.c ../pfstest-values.c ../pic-main.c ../register-core-tests.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/pfstest-core.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F6722
MP_PROCESSOR_OPTION_LD=18f6722
MP_LINKER_DEBUG_OPTION=  -u_DEBUGSTACK
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/core-tests-2.o: ../core-tests-2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests-2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests-2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/core-tests-2.o   ../core-tests-2.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/core-tests-2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/core-tests-2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/core-tests.o: ../core-tests.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/core-tests.o   ../core-tests.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/core-tests.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/core-tests.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o: ../pfstest-alloc-pic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o   ../pfstest-alloc-pic.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o: ../pfstest-arg-handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o   ../pfstest-arg-handler.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o: ../pfstest-arg-handlers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o   ../pfstest-arg-handlers.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-assert.o: ../pfstest-assert.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-assert.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-assert.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-assert.o   ../pfstest-assert.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-assert.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-assert.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-core.o: ../pfstest-core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-core.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-core.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-core.o   ../pfstest-core.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-core.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-list.o: ../pfstest-list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-list.o   ../pfstest-list.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-list.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-matcher.o: ../pfstest-matcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matcher.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-matcher.o   ../pfstest-matcher.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-matcher.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-matcher.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-matchers.o: ../pfstest-matchers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matchers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matchers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-matchers.o   ../pfstest-matchers.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-matchers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-matchers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-mock.o: ../pfstest-mock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-mock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-mock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-mock.o   ../pfstest-mock.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-mock.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-mock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-platform.o: ../pfstest-platform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-platform.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-platform.o   ../pfstest-platform.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-platform.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-platform.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-value.o: ../pfstest-value.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-value.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-value.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-value.o   ../pfstest-value.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-value.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-value.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-values.o: ../pfstest-values.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-values.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-values.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-values.o   ../pfstest-values.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-values.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-values.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pic-main.o: ../pic-main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pic-main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pic-main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pic-main.o   ../pic-main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pic-main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pic-main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/register-core-tests.o: ../register-core-tests.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/register-core-tests.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/register-core-tests.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/register-core-tests.o   ../register-core-tests.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/register-core-tests.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/register-core-tests.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/1472/core-tests-2.o: ../core-tests-2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests-2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests-2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/core-tests-2.o   ../core-tests-2.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/core-tests-2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/core-tests-2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/core-tests.o: ../core-tests.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/core-tests.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/core-tests.o   ../core-tests.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/core-tests.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/core-tests.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o: ../pfstest-alloc-pic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o   ../pfstest-alloc-pic.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-alloc-pic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o: ../pfstest-arg-handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o   ../pfstest-arg-handler.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-arg-handler.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o: ../pfstest-arg-handlers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o   ../pfstest-arg-handlers.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-arg-handlers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-assert.o: ../pfstest-assert.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-assert.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-assert.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-assert.o   ../pfstest-assert.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-assert.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-assert.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-core.o: ../pfstest-core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-core.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-core.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-core.o   ../pfstest-core.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-core.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-list.o: ../pfstest-list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-list.o   ../pfstest-list.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-list.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-matcher.o: ../pfstest-matcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matcher.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-matcher.o   ../pfstest-matcher.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-matcher.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-matcher.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-matchers.o: ../pfstest-matchers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matchers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-matchers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-matchers.o   ../pfstest-matchers.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-matchers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-matchers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-mock.o: ../pfstest-mock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-mock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-mock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-mock.o   ../pfstest-mock.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-mock.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-mock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-platform.o: ../pfstest-platform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-platform.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-platform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-platform.o   ../pfstest-platform.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-platform.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-platform.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-value.o: ../pfstest-value.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-value.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-value.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-value.o   ../pfstest-value.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-value.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-value.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pfstest-values.o: ../pfstest-values.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-values.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pfstest-values.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pfstest-values.o   ../pfstest-values.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pfstest-values.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pfstest-values.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/pic-main.o: ../pic-main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pic-main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pic-main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/pic-main.o   ../pic-main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/pic-main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pic-main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/register-core-tests.o: ../register-core-tests.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/register-core-tests.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/register-core-tests.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/register-core-tests.o   ../register-core-tests.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/register-core-tests.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/register-core-tests.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/pfstest-core.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../18f6722_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "..\18f6722_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/pfstest-core.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/pfstest-core.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../18f6722_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "..\18f6722_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/pfstest-core.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
