#include "xmlparser.h"
#include "fmuWrapper.h"

typedef enum {flat, structured} fmiNamingConvention;
typedef enum {constant, parameter, discrete, continuous} fmiVariability;
typedef enum {input, output, internal, none} fmiCausality;
typedef enum {noalias,alias,negatedAlias} fmiAlias;
typedef enum {fmi_false, fmi_true} fmiBooleanXML;
typedef enum {sv_real, sv_integer, sv_boolean, sv_string,sv_enum} fmiScalarVariableType;

typedef struct {
	const char *name;
	fmiValueReference vr;
	int aliasInd;
	void *next;
} fmuOutputVar;


typedef struct{
	const char *declType;
	const char *start;
	int defStart;
	fmiBooleanXML fixed;
} fmiSTRING;

typedef struct{
	const char *declType;
	fmiBooleanXML start;
	int defStart;
	fmiBooleanXML fixed;
} fmiBOOLEAN;

typedef struct{
	const char *declType;
	const char *quantity;
	int min;
	int defMin;
	int max;
	int defMax;
	int start;
	int defStart;
	fmiBooleanXML fixed;
} fmiINTEGER;

typedef struct{
	const char *declType;
	const char *quantity;
	int min;
	int defMin;
	int max;
	int defMax;
	int start;
	int defStart;
	fmiBooleanXML fixed;
} fmiENUM;

typedef struct{
	const char *declType;
	const char *quantity;
	const char *unit;
	const char *displayUnit;
	fmiBooleanXML relQuantity;
	double min;
	int defMin;
	double max;
	int defMax;
	double nominal;
	int defNorminal;
	double start;
	int defStart;
	fmiBooleanXML fixed;
} fmiREAL;

typedef struct{
	const char  *name;
	char  *flatName;
	fmiValueReference vr; // value reference;
	const char *description;
	fmiVariability var;
	fmiCausality causality;
	fmiAlias alias;
	fmiScalarVariableType type;
	void  *variable;	
} fmiScalarVariable;

typedef struct{
	void *vp;
} fmiArrayVariable;

typedef struct{
	int nsv; // number of scalar variables
	fmiScalarVariable *list_sv;
	int nav; // number of array variables
	fmiArrayVariable *list_av;	
} fmiModelVariable;

typedef struct{
	double startTime;
	double stopTime;
	double tolerance;
} fmiDefaultExperiment;

typedef struct{
	const char *fmiver; // fmi version number;
	const char *mn; // fmi model name
	const char *mid; // model identifier;
	const char *guid; // fingerprint of xml-file content
	const char *description; // string describing the model
	const char *author;
	const char *mver; // model version
	const char *genTool;// generation tool;
	const char *genTime; // generation date and time;
	fmiNamingConvention nconv; // variable naming convention;
	unsigned int ncs; // number of continuous states;
	unsigned int nei; // number of event indicators;
	fmiDefaultExperiment *defaultExperiment;
	fmiModelVariable *modelVariable;
} fmuModelDescription;

char *getFMUname(const char *fmupath);
char *getDllPath(const char *decompPath,const char *mid);
char *getNameXMLfile(const char  *decompPath, const char  *modeldes);
char *getDecompPath(const char  *omPath, const char *mid);
int decompress(const char *fmuPath, const char *decompPath);
int getNumberOfSV(ModelDescription *md);
fmiScalarVariableType getElementType(ScalarVariable *sv);
void *allocateElmSV(fmiScalarVariable fmisv);
void instElmSV(ScalarVariable *sv, fmiScalarVariable fmisv);
void charReplace(char *flatName, unsigned int len, char old, char new);
void instScalarVariable(ModelDescription *md,fmiScalarVariable *list);
fmiNamingConvention getNamingConvention(ModelDescription *md, Att att);
void instFmuModelDescription(ModelDescription *md, fmuModelDescription *fmuMD, fmiModelVariable *fmuMV);
void freeScalarVariableLst(fmiScalarVariable *list,int nsv);
void tmpcodegen(fmuModelDescription */*fmuMD*/, const char */*decompPath*/);
void addOutputVariable(fmiScalarVariable *sv, fmuOutputVar* *root, fmuOutputVar* *nextVar, unsigned int *counter);
char *appendString(char const *head, char const *tail);
void blockcodegen(fmuModelDescription */*fmuMD*/, const char */*decompPath*/, const char */*fmudllpath*/);
void printUsage();