#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<cctype>
#include<assert.h>
using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::vector;
using std::map;
using std::string;
using std::isspace;
typedef string (*func)(vector<string> const &);
void skip_space(istream &in){
    int ch;
    while(isspace(ch=in.get()));
    if(in) in.unget();
}
string awd(map<string,func> const &table,istream &in){
    skip_space(in);
    if(!in) return "";
    char ch;
    in.get(ch);
    if(ch==')') return "";
    if(ch=='\''||ch=='"'){
        string st;st+=ch;
        char nx;
        bool keep=false;
        while(true){
            cin.get(nx);
            st+=nx;
            if(nx=='\\')
                keep=!keep;
            else{
                if(nx==ch&&!keep)
                    break;
                keep=false;
            }
        }
        return st;
    }
    if(ch!='('){
        in.unget();
        string st;
        char ch;
        while(in.get(ch)&&!isspace(ch)&&ch!=')')
            st+=ch;
        if(in) in.unget();
        return st;
    }
    string op;
    cin>>op;
    vector<string> args;
    string t;
    while((t=awd(table,in))!="")
        args.push_back(t);
    if(!table.count(op)){
        string rst=op+"(";
        for(vector<string>::const_iterator it(args.begin());it!=args.end();++it)
            rst+=*it+",";
        rst.back()=')';
        return rst;
    }
    return table.find(op)->second(args);
}
#define fun(name) string name(vector<string> const &args)
#define forarg for(vector<string>::const_iterator it(args.begin());it!=args.end();++it)
fun(plus){
    string st="(";
    forarg
        st+=*it+"+";
    st.back()=')';
    return st;
}
fun(sub){
    string st="(";
    forarg
        st+=*it+"-";
    st.back()=')';
    return st;
}
fun(mul){
    string st="(";
    forarg
        st+=*it+"*";
    st.back()=')';
    return st;
}
fun(div){
    string st="(";
    forarg
        st+=*it+"/";
    st.back()=')';
    return st;
}
fun(mod){
    string st="(";
    forarg
        st+=*it+"%";
    st.back()=')';
    return st;
}
fun(include){
    string st;
    forarg
        st+="#include"+*it+"\n";
    return st;
}
fun(doblock){
    string st="{\n";
    forarg
        st+=*it+";\n";
    return st+"}\n";
}
fun(deffunc){
    assert(args.size()>=3);
    string st=args[0]+" "+args[1]+"(";
    for(vector<string>::const_iterator it(args.begin()+2);it<args.end()-1;++it)
        st+=*it+",";
    if(args.size()>3)
        st.back()=')';
    else
        st+=")";
    st+=args.back();
    return st;
}
fun(def){
    assert(args.size()>=2);
    string st=args[0]+" ";
    for(vector<string>::const_iterator it(args.begin()+1);it<args.end();++it)
        st+=*it+",";
    st.pop_back();
    return st;
}
fun(setvar){
    string st;
    forarg
        st+=*it+"=";
    if(!st.empty())
        st.pop_back();
    return st;
}
map<string,func> regMarco(){
    map<string,func> table;
    table["+"]=plus;
    table["-"]=sub;
    table["*"]=mul;
    table["/"]=div;
    table["%"]=mod;
    table["include"]=include;
    table["do"]=doblock;
    table["func"]=deffunc;
    table["def"]=def;
    table["="]=setvar;
    return table;
}
int main(){
    map<string,func> table=regMarco();
    do{
        string st(awd(table,cin));
        if(st!=""){
            cout<<st;
            if(st.back()!='\n')
                cout<<";\n";
        }
    }while(cin);
    return 0;
}
