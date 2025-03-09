#include<iostream>
#include<fstream>  // for file handling
#include<bitset>  
#include<unordered_map>
using namespace std;

int currLine=0;

// main class for all the instructions
class instructions{
    public:
    bitset<32> instructionIn32bit;   // to store instuction code in binary
    string instructionInHex;  // to store instuction code in hexadecimal
    bitset<7> opcode;    // opcode for the instructions

    // function to convert 32 bit binary to hexadecimal
    void binToHex(){
        instructionInHex="0x";   // initialising with 0x
        // converting 4 bits at a time to hex
        for(int i=31;i>=0;){
            int num=0;
            for(int j=8;j>=1;j/=2){
                num+=instructionIn32bit[i]*j;
                i--;
            }
            if(num<10){
                instructionInHex+=num+'0';
            }else{
                instructionInHex+=num-10+'A';
            }
        }
    }
};

// class label for label in instructions and labels
class label{
    public:

    string name; // label name
    int pc=0;  // goto pc of label

};


// classes for type of instructions

// class for R type instructions inheriting instuctions class publicly
class R:public instructions{
    public:
    bitset<5> rd;       // to store destination register value
    bitset<3> func3;    // to store func3 value
    bitset<5> rs1;      // to store source register 1 value
    bitset<5> rs2;      // to store source register 2 value
    bitset<7> func7;    // to store func7 value
    
    // function to fetch rd,rs1,rs2 fields of the R type instruction
    string fetchInstructionDetails(string line,int i){
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;

        // fetching rd
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int registerValueInDecimal;
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rd=registerValueInDecimal;
        
        // fetching rs1
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs1=registerValueInDecimal;

        // fetching rs2
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        registerValue="";
        while(i<line.size() and line[i]!=' ' and line[i]!='#'){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs2=registerValueInDecimal;
        while(i<line.size() and line[i]!='#'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string detailed_string;
        detailed_string+=opcode.to_string();//opcode
        detailed_string+="-";
        detailed_string+=func3.to_string();//func3
        detailed_string+="-";
        detailed_string+=func7.to_string();//func7
        detailed_string+="-";
        detailed_string+=rd.to_string();//rd
        detailed_string+="-";
        detailed_string+=rs1.to_string();//rs1
        detailed_string+="-";
        detailed_string+=rs2.to_string();//rs2
        detailed_string+="-";
        detailed_string+="NULL";//Immidiate
        return detailed_string;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        for(;idx<12;idx++){
            instructionIn32bit[idx]=rd[idx-7];
        }
        for(;idx<15;idx++){
            instructionIn32bit[idx]=func3[idx-12];
        }
        for(;idx<20;idx++){
            instructionIn32bit[idx]=rs1[idx-15];
        }
        for(;idx<25;idx++){
            instructionIn32bit[idx]=rs2[idx-20];
        }
        for(;idx<32;idx++){
            instructionIn32bit[idx]=func7[idx-25];
        }
        return;
    }

};

// class for I type instructions inheriting instuctions class publicly
class I:public instructions{
    public:
    bitset<5> rd;         // to store destination register value
    bitset<3> func3;      // to store func3 value
    bitset<5> rs1;        // to store source register 1 value
    bitset<12> imm;       // to store immediate field value
    
    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,rs1,imm
        string fetchInstructionDetailsType1(string line,int i){

        // fetching rd
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int registerValueInDecimal;
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rd=registerValueInDecimal;
        
        // fetching rs1
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs1=registerValueInDecimal;

        // fetching imm
        bool neg=false;
        while(!isdigit(line[i])){
            if(line[i]=='-'){
                neg=true;
                i++;
                break;
            }
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string immValue="";
        while(i<line.size() and line[i]!=' ' and line[i]!='#'){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            immValue+=line[i];
            i++;
        }
        if(!isdigit(immValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int immValueInDecimal;
        try
        {
            immValueInDecimal=stoi(immValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(neg==true)  immValueInDecimal=-immValueInDecimal;
        if(immValueInDecimal>2047 or immValueInDecimal<-2048){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        imm=immValueInDecimal;
        while(i<line.size() and line[i]!='#'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string detailed_string;
        detailed_string+=opcode.to_string();//opcode
        detailed_string+="-";
        detailed_string+=func3.to_string();//func3
        detailed_string+="-";
        detailed_string+="NULL";//func7
        detailed_string+="-";
        detailed_string+=rd.to_string();//rd
        detailed_string+="-";
        detailed_string+=rs1.to_string();//rs1
        detailed_string+="-";
        detailed_string+="NULL";//rs2
        detailed_string+="-";
        detailed_string+=imm.to_string();//Immidiate
        return detailed_string;
    }

    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,imm(rs1)
    string fetchInstructionDetailsType2(string line,int i){

        // fetching rd
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int registerValueInDecimal;
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rd=registerValueInDecimal;

        // fetching imm
        bool neg=false;
        while(!isdigit(line[i])){
            if(line[i]=='-'){
                neg=true;
                i++;
                break;
            }
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string immValue="";
        while(i<line.size() and line[i]!=' ' and line[i]!='(' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            immValue+=line[i];
            i++;
        }
        if(!isdigit(immValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int immValueInDecimal;
        try
        {
            immValueInDecimal=stoi(immValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(neg==true)  immValueInDecimal=-immValueInDecimal;
        if(immValueInDecimal>2047 or immValueInDecimal<-2048){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        imm=immValueInDecimal;
        while(i<line.size() and line[i]!='('){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;

        // fetching rs1
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=',' and line[i]!=')'){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        while(line[i]!=')'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs1=registerValueInDecimal;
        while(i<line.size() and line[i]!='#'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string detailed_string;
        detailed_string+=opcode.to_string();//opcode
        detailed_string+="-";
        detailed_string+=func3.to_string();//func3
        detailed_string+="-";
        detailed_string+="NULL";//func7
        detailed_string+="-";
        detailed_string+=rd.to_string();//rd
        detailed_string+="-";
        detailed_string+=rs1.to_string();//rs1
        detailed_string+="-";
        detailed_string+="NULL";//rs2
        detailed_string+="-";
        detailed_string+=imm.to_string();//Immidiate
        return detailed_string;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        for(;idx<12;idx++){
            instructionIn32bit[idx]=rd[idx-7];
        }
        for(;idx<15;idx++){
            instructionIn32bit[idx]=func3[idx-12];
        }
        for(;idx<20;idx++){
            instructionIn32bit[idx]=rs1[idx-15];
        }
        for(;idx<32;idx++){
            instructionIn32bit[idx]=imm[idx-20];
        }
        return;
    }

};

// class for S type instructions inheriting instuctions class publicly
class S:public instructions{
    public:
    bitset<12> imm;     // to store imm value
    bitset<3> func3;    // to store func3 value
    bitset<5> rs1;      // to store source register 1 value
    bitset<5> rs2;      // to store source register 2 value
    
    // function to fetch rs1,imm,rs2 fields of the S type instruction
    string fetchInstructionDetails(string line,int i){
        //fetching rs2
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int registerValueInDecimal;
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs2=registerValueInDecimal;

        // fetching imm
        bool neg=false;
        while(!isdigit(line[i])){
            if(line[i]=='-'){
                neg=true;
                i++;
                break;
            }
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string immValue="";
        while(i<line.size() and line[i]!=' ' and line[i]!='(' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            immValue+=line[i];
            i++;
        }
        if(!isdigit(immValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int immValueInDecimal;
        try
        {
            immValueInDecimal=stoi(immValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(neg==true)  immValueInDecimal=-immValueInDecimal;
        if(immValueInDecimal>2047 or immValueInDecimal<-2048){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        imm=immValueInDecimal;
        while(i<line.size() and line[i]!='('){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;

        // fetching rs1
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=',' and line[i]!=')'){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        while(line[i]!=')'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs1=registerValueInDecimal;
        while(i<line.size() and line[i]!='#'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string detailed_string;
        detailed_string+=opcode.to_string();//opcode
        detailed_string+="-";
        detailed_string+=func3.to_string();//func3
        detailed_string+="-";
        detailed_string+="NULL";//func7
        detailed_string+="-";
        detailed_string+="NULL";//rd
        detailed_string+="-";
        detailed_string+=rs1.to_string();//rs1
        detailed_string+="-";
        detailed_string+=rs2.to_string();//rs2
        detailed_string+="-";
        detailed_string+=imm.to_string();//Immidiate
        return detailed_string;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        for(;idx<12;idx++){
            instructionIn32bit[idx]=imm[idx-7];     // imm[4:0]
        }
        for(;idx<15;idx++){
            instructionIn32bit[idx]=func3[idx-12];
        }
        for(;idx<20;idx++){
            instructionIn32bit[idx]=rs1[idx-15];
        }
        for(;idx<25;idx++){
            instructionIn32bit[idx]=rs2[idx-20];
        }
        for(;idx<32;idx++){
            instructionIn32bit[idx]=imm[idx-20];    // imm[11:5]
        }
        return;
    }

};

// class for SB type instructions inheriting instuctions class publicly
class SB:public instructions{
    public:
    bitset<12> imm;     // to store imm value
    bitset<3> func3;    // to store func3 value
    bitset<5> rs1;      // to store source register 1 value
    bitset<5> rs2;      // to store source register 2 value
    
    // function to fetch rs1,imm,rs2 fields of the SB type instruction
    string fetchInstructionDetails(string line,int i,unordered_map<string,int> labelMap,int currPC){

        //fetching rs1
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int registerValueInDecimal;
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs1=registerValueInDecimal;
        
        // fetching rs2
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rs2=registerValueInDecimal;

        // fetching imm
        while(line[i]==',' or line[i]==' '){
            i++;
        }
        string Label="";
        while(i<line.size() and line[i]!=' ' and line[i]!=',' and line[i]!='#'){
            Label+=line[i];
            i++;
        }
        auto it = labelMap.find(Label);
        if(it!=labelMap.end()){
            int labelPc=it->second;
            imm=(labelPc-currPC)/2;
        }
        else{
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        while(i<line.size() and line[i]!='#'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string detailed_string;
        detailed_string+=opcode.to_string();//opcode
        detailed_string+="-";
        detailed_string+=func3.to_string();//func3
        detailed_string+="-";
        detailed_string+="NULL";//func7
        detailed_string+="-";
        detailed_string+="NULL";//rd
        detailed_string+="-";
        detailed_string+=rs1.to_string();//rs1
        detailed_string+="-";
        detailed_string+=rs2.to_string();//rs2
        detailed_string+="-";
        detailed_string+=imm.to_string();//Immidiate
        return detailed_string;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        instructionIn32bit[idx]=imm[10];      // imm[11]
        idx++;
        for(;idx<12;idx++){
            instructionIn32bit[idx]=imm[idx-8];     // imm[4:1]
        }
        for(;idx<15;idx++){
            instructionIn32bit[idx]=func3[idx-12];
        }
        for(;idx<20;idx++){
            instructionIn32bit[idx]=rs1[idx-15];
        }
        for(;idx<25;idx++){
            instructionIn32bit[idx]=rs2[idx-20];
        }
        for(;idx<31;idx++){
            instructionIn32bit[idx]=imm[idx-21];    // imm[10:5]
        }
        instructionIn32bit[idx]=imm[11];      // imm[12]
        return;
    }

};

// class for U type instructions inheriting instuctions class publicly
class U:public instructions{
    public:
    bitset<5> rd;         // to store destination register value
    bitset<20> imm;       // to store immediate field value
    
    // function to fetch rd,imm fields of the U type instruction of type rd,imm
    string fetchInstructionDetails(string line,int i){

        //fetching rd
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int registerValueInDecimal;
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rd=registerValueInDecimal;

        // fetching imm
        while(!isdigit(line[i])){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string immValue="";
        while(i<line.size() and line[i]!=' ' and line[i]!='#'){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            immValue+=line[i];
            i++;
        }
        if(!isdigit(immValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int immValueInDecimal;
        try
        {
            immValueInDecimal=stoi(immValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(immValueInDecimal>1048575){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        imm=immValueInDecimal;
        while(i<line.size() and line[i]!='#'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string detailed_string;
        detailed_string+=opcode.to_string();//opcode
        detailed_string+="-";
        detailed_string+="NULL";//func3
        detailed_string+="-";
        detailed_string+="NULL";//func7
        detailed_string+="-";
        detailed_string+=rd.to_string();//rd
        detailed_string+="-";
        detailed_string+="NULL";//rs1
        detailed_string+="-";
        detailed_string+="NULL";//rs2
        detailed_string+="-";
        detailed_string+=imm.to_string();//Immidiate
        return detailed_string;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        for(;idx<12;idx++){
            instructionIn32bit[idx]=rd[idx-7];
        }
        for(;idx<32;idx++){
            instructionIn32bit[idx]=imm[idx-12];
        }
        return;
    }

};

// class for UJ type instructions inheriting instuctions class publicly
class UJ:public instructions{
    public:
    bitset<5> rd;    // to store destination register value
    bitset<20> imm;     // to store imm value
    
    // function to fetch rd,imm fields of the UJ type instruction
    string fetchInstructionDetails(string line,int i,unordered_map<string,int> labelMap,int currPC){

        //fetching rd
        while(line[i]!='x'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            if(!isdigit(line[i])){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            registerValue+=line[i];
            i++;
        }
        if(!isdigit(registerValue[0])){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        int registerValueInDecimal;
        try
        {
            registerValueInDecimal=stoi(registerValue);  // string to int conversion
        }
        catch(const std::exception& e)
        {
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        if(registerValueInDecimal>31){
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        rd=registerValueInDecimal;

        // fetching imm
        while(line[i]==',' or line[i]==' '){
            i++;
        }
        string Label="";
        while(i<line.size() and line[i]!=' ' and line[i]!=',' and line[i]!='#'){
            Label+=line[i];
            i++;
        }
        auto it = labelMap.find(Label);
        if(it!=labelMap.end()){
            int labelPc=it->second;
            imm=(labelPc-currPC)/2;
        }
        else{
            cout<<"error at line "<<currLine<<endl;
            exit(-1);
        }
        while(i<line.size() and line[i]!='#'){
            if(line[i]!=' ' and line[i]!=','){
                cout<<"error at line "<<currLine<<endl;
                exit(-1);
            }
            i++;
        }
        string detailed_string;
        detailed_string+=opcode.to_string();//opcode
        detailed_string+="-";
        detailed_string+="NULL";//func3
        detailed_string+="-";
        detailed_string+="NULL";//func7
        detailed_string+="-";
        detailed_string+=rd.to_string();//rd
        detailed_string+="-";
        detailed_string+="NULL";//rs1
        detailed_string+="-";
        detailed_string+="NULL";//rs2
        detailed_string+="-";
        detailed_string+=imm.to_string();//Immidiate
        return detailed_string;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        for(;idx<12;idx++){
            instructionIn32bit[idx]=rd[idx-7];
        }
        for(;idx<20;idx++){
            instructionIn32bit[idx]=imm[idx-1];     //imm[19:12]
        }
        instructionIn32bit[idx]=imm[10];   // imm[11]
        idx++;
        for(;idx<31;idx++){
            instructionIn32bit[idx]=imm[idx-21];    // imm[10:1]
        }
        instructionIn32bit[idx]=imm[19];      // imm[20]
        
        return;
    }

};

// classes for individual instructions

// class for add instructions inheriting R publicly
class add:public R{
    public:
    add(){
        this->opcode=51;
        this->func3=0;
        this->func7=0;
    }
};

// class for and instructions inheriting R publicly
class And:public R{    // since and is a keyword so And name is given
    public:
    And(){
        this->opcode=51;
        this->func3=7;
        this->func7=0;
    }
};

// class for or instructions inheriting R publicly
class Or:public R{    // since or is a keyword so Or name is given
    public:
    Or(){
        this->opcode=51;
        this->func3=6;
        this->func7=0;
    }
};

// class for sll instructions inheriting R publicly
class sll:public R{
    public:
    sll(){
        this->opcode=51;
        this->func3=1;
        this->func7=0;
    }
};

// class for slt instructions inheriting R publicly
class slt:public R{
    public:
    slt(){
        this->opcode=51;
        this->func3=2;
        this->func7=0;
    }
};

// class for sra instructions inheriting R publicly
class sra:public R{
    public:
    sra(){
        this->opcode=51;
        this->func3=5;
        this->func7=32;
    }
};

// class for srl instructions inheriting R publicly
class srl:public R{
    public:
    srl(){
        this->opcode=51;
        this->func3=5;
        this->func7=0;
    }
};

// class for sub instructions inheriting R publicly
class sub:public R{
    public:
    sub(){
        this->opcode=51;
        this->func3=0;
        this->func7=32;
    }
};

// class for xor instructions inheriting R publicly
class Xor:public R{     // since xor is a keyword so Xor name is given
    public:
    Xor(){
        this->opcode=51;
        this->func3=4;
        this->func7=0;
    }
};

// class for mul instructions inheriting R publicly
class mul:public R{
    public:
    mul(){
        this->opcode=51;
        this->func3=0;
        this->func7=1;
    }
};

// class for div instructions inheriting R publicly
class Div:public R{     // since div() is a predefined function so Div name is given
    public:
    Div(){
        this->opcode=51;
        this->func3=4;
        this->func7=1;
    }
};

// class for rem instructions inheriting R publicly
class rem:public R{
    public:
    rem(){
        this->opcode=51;
        this->func3=6;
        this->func7=1;
    }
};

// class for addi instructions inheriting I publicly
class addi: public I{
    public:
    addi(){
        opcode=19;
        func3=0;
    }
};

// class for andi instructions inheriting I publicly
class andi: public I{
    public:
    andi(){
        opcode=19;
        func3=7;
    }
};

// class for ori instructions inheriting I publicly
class ori: public I{
    public:
    ori(){
        opcode=19;
        func3=6;
    }
};

// class for lb instructions inheriting I publicly
class lb: public I{
    public:
    lb(){
        opcode=3;
        func3=0;
    }
};

// class for lh instructions inheriting I publicly
class lh: public I{
    public:
    lh(){
        opcode=3;
        func3=1;
    }
};

// class for lw instructions inheriting I publicly
class lw: public I{
    public:
    lw(){
        opcode=3;
        func3=2;
    }
};

// class for ld instructions inheriting I publicly
class ld: public I{
    public:
    ld(){
        opcode=3;
        func3=3;
    }
};

// class for jalr instructions inheriting I publicly
class jalr: public I{
    public:
    jalr(){
        opcode=103;
        func3=0;
    }
};

// class for sb instructions inheriting S publicly
class sb: public S{
    public:
    sb(){
        opcode=35;
        func3=0;
    }
};

// class for sw instructions inheriting S publicly
class sw: public S{
    public:
    sw(){
        opcode=35;
        func3=2;
    }
};

// class for sd instructions inheriting S publicly
class sd: public S{
    public:
    sd(){
        opcode=35;
        func3=3;
    }
};

// class for sh instructions inheriting S publicly
class sh: public S{
    public:
    sh(){
        opcode=35;
        func3=1;
    }
};

// class for beq instructions inheriting SB publicly
class beq: public SB{
    public:
    beq(){
        opcode=99;
        func3=0;
    }
};

// class for bne instructions inheriting SB publicly
class bne: public SB{
    public:
    bne(){
        opcode=99;
        func3=1;
    }
};

// class for bge instructions inheriting SB publicly
class bge: public SB{
    public:
    bge(){
        opcode=99;
        func3=5;
    }
};

// class for blt instructions inheriting SB publicly
class blt: public SB{
    public:
    blt(){
        opcode=99;
        func3=4;
    }
};

// class for auipc instructions inheriting U publicly
class auipc: public U{
    public:
    auipc(){
        opcode=23;
    }
};

// class for lui instructions inheriting U publicly
class lui: public U{
    public:
    lui(){
        opcode=55;
    }
};

// class for jal instructions inheriting UJ publicly
class jal: public UJ{
    public:
    jal(){
        opcode=111;
    }
};

//class to make machine code for whole program
class makeMC{
    public:

    bitset<32> pc=0;       // to store pc
    string pcInHex;        // to store pc in hex
    unordered_map<string,int> labelMap;     // to store all labels along with their gotopc
    bitset<32> mem=0;      // memory address
    string memInHex;        // to store mem in hex
    unordered_map<string,bitset<32>> memoryMap;     // to store data labels along with their memory

    makeMC(){
        mem[28]=1;
    }

    // function to increment pc by 4
    void incrementPC(bitset<32> &pc){
        for(int i=2;i<32;i++){
            if(pc[i]==0){
                pc[i]=1;
                break;
            }
            else{
                pc[i]=0;
            }
        }
        return;
    }

    // function to increment mem by 1
    void incrementMem(){
        for(int i=0;i<32;i++){
            if(mem[i]==0){
                mem[i]=1;
                break;
            }
            else{
                mem[i]=0;
            }
        }
        return;
    }

    // function to convert 32 bit binary to hexadecimal
    void binToHexPC(){
        if(pc==0){
            pcInHex="0x0";
            return;
        }
        pcInHex="0x";   // initialising with 0x
        bool flag=0;
        // converting 4 bits at a time to hex
        for(int i=31;i>=0;){
            int num=0;
            for(int j=8;j>=1;j/=2){
                num+=pc[i]*j;
                i--;
            }
            if(num!=0)   flag=1;
            if(flag){
                if(num<10){
                    pcInHex+=num+'0';
                }else{
                    pcInHex+=num-10+'A';
                }
            }
        }
        return;
    }

    // function to convert 32 bit binary to hexadecimal
    void binToHexmem(){
        memInHex="0x";   // initialising with 0x
        // converting 4 bits at a time to hex
        for(int i=31;i>=0;){
            int num=0;
            for(int j=8;j>=1;j/=2){
                num+=mem[i]*j;
                i--;
            }
                if(num<10){
                    memInHex+=num+'0';
                }else{
                    memInHex+=num-10+'A';
                }
        }
        return;
    }

    // function to convert 8 bit binary to hexadecimal
    string binToHex(bitset<8> byte){
        string hexa="0x";
        // converting 4 bits at a time to hex
        for(int i=7;i>=0;){
            int num=0;
            for(int j=8;j>=1;j/=2){
                num+=byte[i]*j;
                i--;
            }
            if(num<10){
                hexa+=num+'0';
            }else{
                hexa+=num-10+'A';
            }
        }
        return hexa;
    }

    // to check if the label is correctly made in input.asm
    bool checkLabel(string line){
        int i=line.size();
        while(line[i]!=':'){
            i--;
            if(i<0)  return false;
        }
        return true;
    }

    // to store label name and its corresponding gotopc in labelmap
    void makeLabel(string line){
            int i=0;
            string labelName="";
            while(line[i]!=' ' && line[i]!=':'){
                labelName+=line[i];
                i++;
            }
            while(line[i]!=':'){
                i++;
            }
            label * newLabel = new label();
            newLabel->pc=this->pc.to_ulong();
            newLabel->name=labelName;
            labelMap[labelName]=newLabel->pc;
            for(int j=i+1;j<line.size();j++){
                if(line[j]=='#') break;
                if(line[j]!=' '){
                    string subLine=line.substr(j);
                    if(checkDirectives(subLine)) {
                        workOnDirectives(subLine);
                        break;
                    }
                    else{ 
                        if(!checkComment(subLine));
                        incrementPC(pc);
                        break;
                    }
                }  
            }
        return;
    }

    bool checkDirectives(string line){
        int i=0;
        while(line[i]==' ')  i++;
        string directive="";
        while(line[i]!=' '){
            directive+=line[i];
            i++;
        }
        if(directive == ".byte" || directive == ".half" || directive == ".word" || directive == ".dword" || directive == ".asciz") return true;
        else return false;
    }

    void makeDataLabel(string line){
        int i=0;
            string labelName="";
            while(line[i]!=' ' && line[i]!=':'){
                labelName+=line[i];
                i++;
            }
            while(line[i]!=':'){
                i++;
            }
            for(int j=i+1;j<line.size();j++){
                if(line[j]=='#') break;
                
                if(line[j]!=' '){
                    string subLine=line.substr(j);
                    if(!checkDirectives(subLine)) {
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                        return;
                    }
                    else{ 
                        workOnDirectives(subLine);
                        break;
                    }
                }  
            }
            memoryMap[labelName]=mem;
        return;
    }

    void workOnDirectives(string line){
        int i=0;
        while(line[i]==' ')  i++;
        string directive="";
        while(line[i]!=' '){
            directive+=line[i];
            i++;
        }
        if(directive==".byte"){
            ofstream fout;   // to write to a file
            fout.open("output.mc",std::ios::app);     // open output.mc to write
            while(i<line.size()){
                bitset<8> byte;
                int val;
                string t="";
                while(i<line.size() and (line[i]==' ' or line[i]==',')){
                    i++;
                }
                if(i==line.size())  break;
                bool neg=false;
                if(line[i]=='-'){
                    neg=true;
                    i++;
                }
                if(line[i]=='#') break;
                while(i<line.size() and line[i]!=' ' and line[i]!=',' and line[i]!='#'){
                    if(!isdigit(line[i])){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    t+=line[i];
                    i++;
                }
                try
                {
                    val=stoi(t);
                }
                catch(const std::exception& e)
                {
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                if(neg==true) val=-val;
                if(val>127 or val<-128){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                byte=val;
                binToHexmem();
                fout<<memInHex<<" "<<binToHex(byte)<<endl;
                incrementMem();
            }
            fout.close();
        }
        else if(directive==".half"){
            ofstream fout;   // to write to a file
            fout.open("output.mc",std::ios::app);     // open output.mc to write
            while(i<line.size()){
                bitset<8> byte;
                bitset<16>half;
                int val;
                string t="";
                while(i<line.size() and (line[i]==' ' or line[i]==',')){
                    i++;
                }
                if(i==line.size())  break;
                bool neg=false;
                if(line[i]=='-'){
                    neg=true;
                    i++;
                }
                if(line[i]=='#') break;
                while(i<line.size() and line[i]!=' ' and line[i]!=',' and line[i]!='#'){
                    if(!isdigit(line[i])){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    t+=line[i];
                    i++;
                }
                try
                {
                    val=stoi(t);
                }
                catch(const std::exception& e)
                {
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                if(neg==true) val=-val;
                if(val>32767 or val<-32768){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                half=val;
                for(int j=0;j<2;j++){
                    for(int k=0;k<8;k++){
                        byte[k]=half[8*j+k];
                    }
                    binToHexmem();
                    fout<<memInHex<<" "<<binToHex(byte)<<endl;
                    incrementMem();
                }
            }
            fout.close();
        }
        else if(directive==".word"){
            ofstream fout;   // to write to a file
            fout.open("output.mc",std::ios::app);     // open output.mc to write
            while(i<line.size()){
                bitset<8> byte;
                bitset<32>word;
                unsigned int val;
                string t="";
                while(i<line.size() and (line[i]==' ' or line[i]==',')){
                    i++;
                }
                if(i==line.size())  break;
                bool neg=false;
                if(line[i]=='-'){
                    neg=true;
                    i++;
                }
                if(line[i]=='#') break;
                while(i<line.size() and line[i]!=' ' and line[i]!=',' and line[i]!='#'){
                    if(!isdigit(line[i])){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    t+=line[i];
                    i++;
                }
                try
                {
                    val=stoul(t);
                }
                catch(const std::exception& e)
                {
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                if(neg==true){
                    if(val>2147483648){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    else{
                        val=-val;
                    }
                }
                else if(val>2147483647){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                word=val;
                for(int j=0;j<4;j++){
                    for(int k=0;k<8;k++){
                        byte[k]=word[8*j+k];
                    }
                    binToHexmem();
                    fout<<memInHex<<" "<<binToHex(byte)<<endl;
                    incrementMem();
                }
            }
            fout.close();
        }
        else if(directive==".dword"){
            ofstream fout;   // to write to a file
            fout.open("output.mc",std::ios::app);     // open output.mc to write
            while(i<line.size()){
                bitset<8> byte;
                bitset<64>dword;
                unsigned long long int val;
                string t="";
                while(i<line.size() and (line[i]==' ' or line[i]==',')){
                    i++;
                }
                if(i==line.size())  break;
                bool neg=false;
                if(line[i]=='-'){
                    neg=true;
                    i++;
                }
                if(line[i]=='#') break;
                while(i<line.size() and line[i]!=' ' and line[i]!=',' and line[i]!='#'){
                    if(!isdigit(line[i])){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    t+=line[i];
                    i++;
                }
                try
                {
                    val=stoull(t);
                }
                catch(const std::exception& e)
                {
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                if(neg==true){
                    if(val>9223372036854775808){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    else{
                        val=-val;
                    }
                }
                else if(val>9223372036854775807){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                dword=val;
                for(int j=0;j<8;j++){
                    for(int k=0;k<8;k++){
                        byte[k]=dword[8*j+k];
                    }
                    binToHexmem();
                    fout<<memInHex<<" "<<binToHex(byte)<<endl;
                    incrementMem();
                }
            }
            fout.close();
        }
        else if(directive==".asciz"){
            ofstream fout;   // to write to a file
            fout.open("output.mc",std::ios::app);     // open output.mc to write
                bitset<8> byte;
                string t="";
                while(i<line.size() and line[i]!='\"'){
                    if(i<line.size() and line[i]!=' ' and line[i]!=','){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    i++;
                }
                if(line[i]!='\"'){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                i++;
                while(i<line.size() and line[i]!='\"'){
                    t+=line[i];
                    i++;
                }
                if(line[i]!='\"'){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                i++;
                while(i<line.size()){
                    if(line[i]!=' ' and line[i]!=','){
                        cout<<"error at line "<<currLine<<endl;
                        exit(-1);
                    }
                    i++;
                }
                char ch=t[0];
                int z=0;
                while(ch!='\0'){
                    z++;
                    byte=ch;
                    binToHexmem();
                    fout<<memInHex<<" "<<binToHex(byte)<<endl;
                    incrementMem();
                    ch=t[z];
                }
            fout.close();
        }
    }

    bool checkDataDirective(string line){
        int i=0;
        while(line[i]==' ')  i++;
        string instruct="";
        while(i<line.size() and line[i]!=' '){
            instruct+=line[i];
            i++;
        }
        if(instruct==".data"){
            return true;
        }
        return false;
    }

    bool checkTextDirective(string line){
        int i=0;
        while(line[i]==' ')  i++;
        string instruct="";
        while(i<line.size() and line[i]!=' '){
            instruct+=line[i];
            i++;
        }
        if(instruct==".text"){
            return true;
        }
        return false;
    }

    bool checkComment(string line){
        int i=0;
        while(line[i]==' ' or line[i]==',')  i++;
        // fetching instruction name in instruct
        string instruct="";
        while(i<line.size() and line[i]!=' ' and line[i]!=','){
            instruct+=line[i];
            i++;
        }
        if(instruct[0]=='#' or instruct=="\0"){
            return "true";
        }
        return false;
    }

    // function to check if the instruction is right or not and which instruction is it
    pair<string,string> checkInstruction(string line){
        int i=0;
        while(line[i]==' ' or line[i]==',')  i++;
        // fetching instruction name in instruct
        string instruct="";
        while(i<line.size() and line[i]!=' ' and line[i]!=','){
            instruct+=line[i];
            i++;
        }

        if(instruct[0]=='#' or instruct=="\0"){
            return {"comment",""};
        }

        else if(instruct=="add"){
            add *addInstruction = new add();    // making an object of class add
            string detailed_string=addInstruction->fetchInstructionDetails(line,i);  // fetching fields
            addInstruction->convertTo32bit();   // converting to 32 bit code
            addInstruction->binToHex();   // converting to hex
            return {addInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if(instruct=="and"){
            And *andInstruction = new And();    // making an object of class And
            string detailed_string=andInstruction->fetchInstructionDetails(line,i);    // fetching fields
            andInstruction->convertTo32bit();   // converting to 32 bit code
            andInstruction->binToHex();     // converting to hex
            return {andInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if(instruct=="or"){
            Or *newInstruction = new Or();    // making an object of class Or
            string detailed_string=newInstruction->fetchInstructionDetails(line,i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if(instruct=="or"){
            Or *newInstruction = new Or();    // making an object of class And
            string detailed_string=newInstruction->fetchInstructionDetails(line,i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "sll") {
            sll *newInstruction = new sll();    // making an object of class sll
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "slt") {
            slt *newInstruction = new slt();    // making an object of class slt
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "sra") {
            sra *newInstruction = new sra();    // making an object of class sra
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "srl") {
            srl *newInstruction = new srl();    // making an object of class srl
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "sub") {
            sub *newInstruction = new sub();    // making an object of class sub
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "xor") {
            Xor *newInstruction = new Xor();    // making an object of class Xor
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "mul") {
            mul *newInstruction = new mul();    // making an object of class mul
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "div") {
            Div *newInstruction = new Div();    // making an object of class Div
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "rem") {
            rem *newInstruction = new rem();    // making an object of class rem
            string detailed_string=newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return {newInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "addi") {
            addi *addiInstruction = new addi();    // making an object of class addi
            string detailed_string=addiInstruction->fetchInstructionDetailsType1(line, i);   // fetching fields
            addiInstruction->convertTo32bit();  // converting to 32-bit code
            addiInstruction->binToHex();    // converting to hex
            return {addiInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if(instruct=="andi"){
            andi *andiInstruction = new andi();    // making an object of class andi
            string detailed_string=andiInstruction->fetchInstructionDetailsType1(line,i);   // fetching fields
            andiInstruction->convertTo32bit();  // converting to 32 bit code
            andiInstruction->binToHex();    // converting to hex
            return {andiInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "ori") {
            ori *oriInstruction = new ori();    // making an object of class ori
            string detailed_string=oriInstruction->fetchInstructionDetailsType1(line, i);   // fetching fields
            oriInstruction->convertTo32bit();  // converting to 32-bit code
            oriInstruction->binToHex();    // converting to hex
            return {oriInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "lb") {
            lb *lbInstruction = new lb();    // making an object of class lb
            string detailed_string=lbInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            lbInstruction->convertTo32bit();  // converting to 32-bit code
            lbInstruction->binToHex();    // converting to hex
            return {lbInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "ld") {
            ld *ldInstruction = new ld();    // making an object of class ld
            string detailed_string=ldInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            ldInstruction->convertTo32bit();  // converting to 32-bit code
            ldInstruction->binToHex();    // converting to hex
            return {ldInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "lh") {
            lh *lhInstruction = new lh();    // making an object of class lh
            string detailed_string=lhInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            lhInstruction->convertTo32bit();  // converting to 32-bit code
            lhInstruction->binToHex();    // converting to hex
            return {lhInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "lw") {
            lw *lwInstruction = new lw();    // making an object of class lw
            string detailed_string=lwInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            lwInstruction->convertTo32bit();  // converting to 32-bit code
            lwInstruction->binToHex();    // converting to hex
            return {lwInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "jalr") {
            jalr *jalrInstruction = new jalr();    // making an object of class jalr
            string detailed_string=jalrInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            jalrInstruction->convertTo32bit();  // converting to 32-bit code
            jalrInstruction->binToHex();    // converting to hex
            return {jalrInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "sb") {
            sb *sbInstruction = new sb();    // making an object of class sb
            string detailed_string=sbInstruction->fetchInstructionDetails(line, i);   // fetching fields
            sbInstruction->convertTo32bit();  // converting to 32-bit code
            sbInstruction->binToHex();    // converting to hex
            return {sbInstruction->instructionInHex,detailed_string};   // returning mc in hex
            } 
            
        else if (instruct == "sw") {
            sw *swInstruction = new sw();    // making an object of class sw
            string detailed_string=swInstruction->fetchInstructionDetails(line, i);   // fetching fields
            swInstruction->convertTo32bit();  // converting to 32-bit code
            swInstruction->binToHex();    // converting to hex
            return {swInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "sd") {
            sd *sdInstruction = new sd();    // making an object of class sd
            string detailed_string=sdInstruction->fetchInstructionDetails(line, i);   // fetching fields
            sdInstruction->convertTo32bit();  // converting to 32-bit code
            sdInstruction->binToHex();    // converting to hex
            return {sdInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "sh") {
            sh *shInstruction = new sh();    // making an object of class sh
            string detailed_string=shInstruction->fetchInstructionDetails(line, i);   // fetching fields
            shInstruction->convertTo32bit();  // converting to 32-bit code
            shInstruction->binToHex();    // converting to hex
            return {shInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "beq") {
            beq *beqInstruction = new beq();    // making an object of class beq
            string detailed_string=beqInstruction->fetchInstructionDetails(line, i ,labelMap,this->pc.to_ulong());   // fetching fields
            beqInstruction->convertTo32bit();  // converting to 32-bit code
            beqInstruction->binToHex();    // converting to hex
            return {beqInstruction->instructionInHex,detailed_string};   // returning mc in hex
        }

        else if (instruct == "bne") {
            bne *bneInstruction = new bne();  // Creating a new object of type bne
            string detailed_string=bneInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            bneInstruction->convertTo32bit(); // Converting instruction to 32-bit code
            bneInstruction->binToHex(); // Converting binary code to hexadecimal
            return {bneInstruction->instructionInHex,detailed_string}; // Returning the instruction in hexadecimal format
        }

        else if (instruct == "bge") {
            bge *bgeInstruction = new bge();  // Creating a new object of type bge
            string detailed_string=bgeInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            bgeInstruction->convertTo32bit(); // Converting instruction to 32-bit code
            bgeInstruction->binToHex(); // Converting binary code to hexadecimal
            return {bgeInstruction->instructionInHex,detailed_string}; // Returning the instruction in hexadecimal format
        }

        else if (instruct == "blt") {
            blt *bltInstruction = new blt();  // Creating a new object of type blt
            string detailed_string=bltInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            bltInstruction->convertTo32bit(); // Converting instruction to 32-bit code
            bltInstruction->binToHex(); // Converting binary code to hexadecimal
            return {bltInstruction->instructionInHex,detailed_string}; // Returning the instruction in hexadecimal format
        }

        else if (instruct == "auipc") {
            auipc *auipcInstruction = new auipc();  // Creating a new object of type auipc
            string detailed_string=auipcInstruction->fetchInstructionDetails(line, i); // Fetching instruction details
            auipcInstruction->convertTo32bit(); // Converting instruction to 32-bit code
            auipcInstruction->binToHex(); // Converting binary code to hexadecimal
            return {auipcInstruction->instructionInHex,detailed_string}; // Returning the instruction in hexadecimal format
        }

        else if (instruct == "lui") {
            lui *luiInstruction = new lui();  // Creating a new object of type lui
            string detailed_string=luiInstruction->fetchInstructionDetails(line, i); // Fetching instruction details
            luiInstruction->convertTo32bit(); // Converting instruction to 32-bit code
            luiInstruction->binToHex(); // Converting binary code to hexadecimal
            return {luiInstruction->instructionInHex,detailed_string}; // Returning the instruction in hexadecimal format
        }

        else if (instruct == "jal") {
            jal *jalInstruction = new jal();  // Creating a new object of type jal
            string detailed_string=jalInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            jalInstruction->convertTo32bit(); // Converting instruction to 32-bit code
            jalInstruction->binToHex(); // Converting binary code to hexadecimal
            return {jalInstruction->instructionInHex,detailed_string}; // Returning the instruction in hexadecimal format
        }

        else return {"error",""};   // return error if instruction doesnot match to any name
    }
    //function to remove unnecessary spaces in the instruction
    string clean_string(string s){
        string t,r;

        int i=0;
        for(int j=0;j<s.size();j++){
            if(s[j]!=' '){
                i=j;
                break;
            }
        }

        t=s.substr(i,s.size()-i);

        int j=0;
        while(t[j]!=' ' && j<t.size())j++;
        j++;

        r=t.substr(0,j);
        for(int k=j;k<t.size();k++){
            if(t[k]!=' ')r+=t[k];
        }

        return r;
    }
    // function to read from .asm file and write to .mc file
    void readAndWrite(){
        ifstream fin;    // to read a file
        ofstream fout;   // to write to a file
        string line;

        fout.open("output.mc");
        fout.close();

        fin.open("input.asm");       // open input.asm to read
        // checking all labels only and storing them
        while(getline(fin,line)){
            currLine++;
            if(checkDataDirective(line)){
                if(!fin.eof()){
                    getline(fin,line);
                    currLine++;
                }  
                while(!checkTextDirective(line)){
                    if(checkDirectives(line)) workOnDirectives(line);
                    else if(checkLabel(line)) makeDataLabel(line);
                    else{
                        int i=0;
                        while(i<line.size() and line[i]==' '){
                            i++;
                        }
                        if(line[i]!='#' and line[i]!='\0'){
                            cout<<"error at line "<<currLine<<endl;
                            exit(-1);
                        }
                    }
                    if(!fin.eof()){
                        getline(fin,line);
                        currLine++;
                    }
                    else break;
                    
                }
            }
            else if(checkDirectives(line)){
                workOnDirectives(line);
            }
            else if(checkTextDirective(line)){
                continue;
            }
            else if(checkLabel(line)){
                makeLabel(line);
            }
            else if(checkComment(line)){
                continue;
            }
            else  incrementPC(this->pc);
        }
        fin.close();    // close input.asm

        this->pc=0;
        currLine=0;

        fin.open("input.asm");       // open input.asm to read
        fout.open("output.mc",std::ios::app);     // open output.mc to write
        fout<<endl;
        if(!fout){
            cout<<"error";
        }
        // checking instructions only (not labels)
        while(getline(fin,line)){
            currLine++;
            if(checkComment(line)){
                continue;
            }
            else if(checkDataDirective(line)){
                while(!fin.eof() and !checkTextDirective(line)){   
                    getline(fin,line);
                    currLine++;
                }              
            }
            else if(checkDirectives(line)){
                continue;
            }
            else if(checkTextDirective(line)){
                continue;
            }
            else if(checkLabel(line)){
                int i=0;
                while(line[i]!=':') i++;
                i++;
                string subLine=line.substr(i);
                string clean=clean_string(subLine);
                if(checkComment(subLine)){
                    continue;
                }
                else if(checkDataDirective(subLine)){
                    while(!fin.eof() and !checkTextDirective(line)){   
                        getline(fin,line);
                        currLine++;
                    }              
                }
                else if(checkDirectives(subLine)){
                    continue;
                }
                else if(checkTextDirective(subLine)){
                    continue;
                }

                if(checkInstruction(subLine).first!="error" and checkInstruction(subLine).first!="comment"){
                    binToHexPC();
                    fout<<pcInHex<<" "<<checkInstruction(subLine).first<<" , "<<clean<<" # "<<checkInstruction(subLine).second<<endl;      // writing to output.mc
                    incrementPC(this->pc);
                }
                else if(checkInstruction(subLine).first=="error"){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
            }
            else if(!checkLabel(line)){
                if(checkInstruction(line).first=="error"){
                    cout<<"error at line "<<currLine<<endl;
                    exit(-1);
                }
                binToHexPC();
                string clean=clean_string(line);
                fout<<pcInHex<<" "<<checkInstruction(line).first<<" , "<<clean<<" # "<<checkInstruction(line).second<<endl;      // writing to output.mc
                incrementPC(this->pc);
            }
        }
        binToHexPC();
        fout<<pcInHex<<" "<<"0x00000000"<<endl;
        fin.close();    // close input.asm
        fout.close();   // close output.mc
    }
};

int main(){
    makeMC *newMC=new makeMC();    // making object for makeMC class
    newMC->readAndWrite();      // makes output.mc taking inputs from input.asm
    return 0;
}
