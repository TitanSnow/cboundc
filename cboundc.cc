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
        while(in.get(nx)){
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
        char nx;
        bool keep=false;
        while(in.get(nx)){
            st+=nx;
            if(nx=='\\')
                keep=!keep;
            else{
                if(isspace(nx)){
                    st.pop_back();
                    if(!keep)
                        break;
                    else
                        st.back()=' ';
                }else if(nx==')'){
                    st.pop_back();
                    break;
                }
                keep=false;
            }
        }
        if(in) in.unget();
        return st;
    }
    string op;
    string &st=op;
    char nx;
    bool keep=false;
    while(in.get(nx)){
        st+=nx;
        if(nx=='\\')
            keep=!keep;
        else{
            if(isspace(nx)){
                st.pop_back();
                if(!keep)
                    break;
                else
                    st.back()=' ';
            }else if(nx==')'){
                st.pop_back();
                break;
            }
            keep=false;
        }
    }
    if(in) in.unget();
    vector<string> args;
    string t;
    while((t=awd(table,in))!="")
        args.push_back(t);
    if(!table.count(op)){
        string rst=op+"(";
        for(vector<string>::const_iterator it(args.begin());it!=args.end();++it)
            rst+=*it+",";
        if(!args.empty())rst.back()=')';
        else rst+=")";
        return rst;
    }
    return table.find(op)->second(args);
}
#define fun(name) string name(vector<string> const &args)
#define forarg for(vector<string>::const_iterator it(args.begin());it!=args.end();++it)
fun(plus){
    assert(!args.empty());
    if(args.size()==1) return string("+(")+args.front()+")";
    string st="(";
    forarg
        st+=*it+"+";
    st.back()=')';
    return st;
}
fun(sub){
    assert(!args.empty());
    if(args.size()==1) return string("-(")+args.front()+")";
    string st="(";
    forarg
        st+=*it+"-";
    st.back()=')';
    return st;
}
fun(mul){
    assert(!args.empty());
    if(args.size()==1) return string("*(")+args.front()+")";
    string st="(";
    forarg
        st+=*it+"*";
    st.back()=')';
    return st;
}
fun(div){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+"/";
    st.back()=')';
    return st;
}
fun(mod){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+"%";
    st.back()=')';
    return st;
}
fun(include){
    assert(!args.empty());
    string st;
    forarg
        st+="#include"+*it+"\n";
    return st;
}
fun(doblock){
    string st="{\n";
    forarg
        st+=*it+(it->back()=='\n'?"":";\n");
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
    assert(!args.empty());
    string st;
    forarg
        st+=*it+"=";
    if(!st.empty())
        st.pop_back();
    return st;
}
fun(ifelse){
    string st("if(");
    assert(args.size()==2||args.size()==3);
    st+=args[0]+")"+args[1];
    if(args.size()==3){
        if(st.back()!='\n')
            st+=";";
        st+="else ";
        st+=args[2];
    }
    return st;
}
fun(lhs){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+"<<";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(rhs){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+">>";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(lt){
    assert(args.size()>=2);
    string st="(";
    for(vector<string>::const_iterator it(args.begin());it!=args.end()-1;++it)
        st+="("+*it+"<"+*(it+1)+")&&";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(rt){
    assert(args.size()>=2);
    string st="(";
    for(vector<string>::const_iterator it(args.begin());it!=args.end()-1;++it)
        st+="("+*it+">"+*(it+1)+")&&";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(lte){
    assert(args.size()>=2);
    string st="(";
    for(vector<string>::const_iterator it(args.begin());it!=args.end()-1;++it)
        st+="("+*it+"<="+*(it+1)+")&&";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(rte){
    assert(args.size()>=2);
    string st="(";
    for(vector<string>::const_iterator it(args.begin());it!=args.end()-1;++it)
        st+="("+*it+">="+*(it+1)+")&&";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(eq){
    assert(args.size()>=2);
    string st="(";
    for(vector<string>::const_iterator it(args.begin());it!=args.end()-1;++it)
        st+="("+*it+"=="+*(it+1)+")&&";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(neq){
    assert(args.size()>=2);
    string st="(";
    for(vector<string>::const_iterator it(args.begin());it!=args.end()-1;++it)
        st+="("+*it+"!="+*(it+1)+")&&";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(band){
    assert(!args.empty());
    if(args.size()==1) return string("&(")+args.front()+")";
    string st="(";
    forarg
        st+=*it+"&";
    st.back()=')';
    return st;
}
fun(bxor){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+"^";
    st.back()=')';
    return st;
}
fun(bor){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+"|";
    st.back()=')';
    return st;
}
fun(land){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+"&&";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(lor){
    assert(!args.empty());
    string st="(";
    forarg
        st+=*it+"||";
    st.pop_back();
    st.back()=')';
    return st;
}
fun(forblock){
    assert(args.size()==4);
    return string("for(")+args[0]+";"+args[1]+";"+args[2]+")"+args[3];
}
fun(cont){
    assert(args.empty());
    return "continue";
}
fun(brk){
    assert(args.empty());
    return "break";
}
fun(empty){
    assert(args.empty());
    return " ";
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
    table["if"]=ifelse;
    table["<<"]=lhs;
    table[">>"]=rhs;
    table["<"]=lt;
    table[">"]=rt;
    table["<="]=lte;
    table[">="]=rte;
    table["=="]=eq;
    table["!="]=neq;
    table["&"]=band;
    table["^"]=bxor;
    table["|"]=bor;
    table["and"]=land;
    table["or"]=lor;
    table["for"]=forblock;
    table["continue"]=cont;
    table["break"]=brk;
    table["empty"]=empty;
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
