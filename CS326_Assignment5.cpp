 //Calvin Brooks
#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <iomanip>

using namespace std;

vector<string> tokenType;
vector<string> tokenValue;
vector<int> eosLocations;//holds indexes of all the eos' in the tokenType vector
int statementCursor = 0;//moves through statements in parser keeping track of word location
int errorCount = 0;//used to debug-returns the 

bool isIdentifier(string first)
{
    if(first[0] < 97 || first[0] > 122)
    {
        if(first[0] != '_')
	    {
	        return false;//not valid identifier
	    }
    }
    for(int i = 1; i < first.length(); i++)
    {
        if(first[i] < 65)
        {
            if(first[i] < 48 || first[i] > 57)
            {
                if(first[i] != '_')
                {
                    return false;//not valid identifier
                }
            }
        }
        
        if(first[i] > 90)
        {
            if(first[i] < 97 || first[i] > 122)
            {
                if(first[i] != '_')
                {
                    return false;//not valid identifier
                }
            }
        }
    }
    //keyword cannot be identifier
    if(first == "decimal" || first == "integer" || first == "string" || first == "string" || first == "define" || first == "read")
    {
        return false;
    }
    return true;
}

bool isEOS(string first)
{
    char eos = first.at(first.length() - 1);
    if(eos == ';')
    {
        return true;
    }
    return false;
}
bool isStringIdentifier(string first)
{
    char slash = first.at(0);
    if(slash == 92)
    {
        return true;
    }
    return false;
}
bool isString(string first)
{
    char quotes = first.at(0);
    char quotes2 = first.at(first.length() - 1);
    if(quotes == 34 || quotes == 39 || quotes2 == 34 || quotes2 == 39)
    {
        return true;
    }
    return false;
}
bool isComment(string first)
{
    char comment = first.at(0);
    char comment2 = first.at(first.length() - 1);
    if(comment == '#' || comment2 == '#')
    {
        return true;
    }
    return false;
}

bool isKeyword(string first)
{
    if(first == "decimal" || first == "integer" || first == "string" || first == "print" || first == "define" || first == "read" || first == "bool")
    {
        return true;
    }
    return false;
}
bool isOperator(string first)
{
	if(first == "+" || first == "-" || first == "*" || first == "/" || first == "=" || first == "==")
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool isDecimal(string first)
{
    bool decimalFlag = false;//true when decimal found
    for(int i = 0; i < first.length(); i++)
    {
        if(first[i] < 48 || first[i] > 57)
        {
           if(first[i] == '.')
           {
               if(decimalFlag != false)
               {
                   return false;
               }
               else
               {
                   decimalFlag = true;
               }
           }
           else
           {
               return false;
           }
        }
    }
    
    if(decimalFlag == true)
    {
        return true;
    }
    return false;
}
bool isNonDecimal(string first)
{
    for(int i = 0; i < first.length(); i++)
    {
        if(first[i] < 48 || first[i] > 57)
        {
            return false;
        }
    }
    return true;
}
bool isTrueOrFalse(string first)
{
    if(first == "true" || first == "false")
    {
        return true;
    }
    else
    {
        return false;
    }
}
void lexer(string fileName)
{
    bool stringFlag = false;
    bool commentFlag = false;
    string wholeString;
    fstream source;
    string word;
    source.open(fileName.c_str());
    while(source >> word)
    {   
        //first and foremost ignore any comments-unless string ... 
        if(isComment(word) == true)
        {
            if(stringFlag == false)//comment if not in string
            {
            	if(commentFlag == false)//comments beginning
            	{
                	commentFlag = true;
                	continue;
            	}
            }

        }
        
        if(commentFlag == true)//if still in comment
        {
            if(isComment(word) == false)//comment hasnt ended
            {
                continue;
            }
            else//comment has ended
            {
                commentFlag = false;
                continue;
            }
        }
        //realize an EOS immediately so we can split the word from ';'
        if(isEOS(word) == true)
        {
            if(stringFlag == true)//string would automatically end
            {
                if(word.length() > 2)
                {
                    stringFlag = false;//extract quote AND eos from word
                    string last = word.substr(0, word.length() - 2);
                    string fluf = " ";
                    last += fluf;
                    //last is part of a wholeString that just ended 
                    wholeString += last;
                    if(wholeString != "")
                    {
                        tokenType.push_back("string");
                        tokenValue.push_back(wholeString);
                        wholeString = "";
                    }
                    tokenType.push_back("eos");
                    tokenValue.push_back(";");
                    wholeString = "";
                    continue;
                }
                else//if word is "; or ';
                {
                    stringFlag = false;
                    if(wholeString != "")
                    {
                        tokenType.push_back("string");
                        tokenValue.push_back(wholeString);
                        wholeString = "";
                    }
                    wholeString = "";
                    tokenType.push_back("eos");
                    tokenValue.push_back(";");
                    continue;
                }

            }
            if(stringFlag == false)
            {
                //only need to extract eos and word
                string last = word.substr(0, word.length() - 1);
                if(isTrueOrFalse(last) == true)
                {
                    tokenType.push_back("boolean");
			        tokenValue.push_back(last);
                    tokenType.push_back("eos");
                    tokenValue.push_back(";");
                    continue; 
                }
                if(isIdentifier(last) == true)
                {
                    tokenType.push_back("identifier");
                    tokenValue.push_back(last);
                    tokenType.push_back("eos");
                    tokenValue.push_back(";");
                    continue;                    
                }
                if(isDecimal(last) == true)
                {
                    tokenType.push_back("decimal");
                    tokenValue.push_back(last);
                    tokenType.push_back("eos");
                    tokenValue.push_back(";");
                    continue;                     
                }
                if(isNonDecimal(last) == true)
                {
                    tokenType.push_back("nondecimal");
                    tokenValue.push_back(last);
                    tokenType.push_back("eos");
                    tokenValue.push_back(";");
                    continue;  
                }
                
            }
        }
        //realize if we're in a string so we wont check for any expression
        if(stringFlag == true)
        {
            if(isStringIdentifier(word) == true)//extract the word
            {
                //add identifier AND add previous strings
                string id = word.substr(1, word.length());
                if(wholeString != "")
                {
                    tokenType.push_back("string");
                    tokenValue.push_back(wholeString);
                    wholeString = "";
                }
                tokenType.push_back("identifier");
                tokenValue.push_back(id);
                continue;
            }
            if(isString(word) == true)//string ends
            {
                //strings done, add to vector and change string flag
                string subword0 = word.substr(0, (word.length() - 1));
                string fluf = " ";
                subword0 += fluf;
                wholeString += subword0;
                stringFlag = false;//strings done
                if(wholeString != "")
                {
                    tokenType.push_back("string");
                    tokenValue.push_back(wholeString);
                    wholeString = "";
                }
                wholeString = "";
                continue;//!!!!!!!!!
            }
            else
            {
                string fluf = " ";
                word += fluf;
                wholeString += word;//if string doesnt end, keep adding to string 
                continue;
            }
        }
        //realize if a string has started or ended so we can flag a string
        if(isString(word) == true)
        {
            if(stringFlag == false)//string begins
            {
                stringFlag = true;
                string subword = word.substr(1, word.length());
                string fluf = " ";
                subword += fluf;
                wholeString += subword;
                continue;//continue to grab next word-will be flagged as string
            }
        }
        //keyword holds priority
        if(isKeyword(word) == true)
        {
            tokenType.push_back("keyword");
			tokenValue.push_back(word);
			continue;
        }
        if(isTrueOrFalse(word) == true)
        {
            tokenType.push_back("boolean");
			tokenValue.push_back(word);
			continue;
        }
        //identifier
        if(isIdentifier(word) == true)
        {
            tokenType.push_back("identifier");
			tokenValue.push_back(word);
			continue;
        }
        //operator
        if(isOperator(word) == true)
        {
            if(stringFlag == false)
            {
                tokenType.push_back("operator");
			    tokenValue.push_back(word);
			    continue;
            }
            if(stringFlag == true)
            {
                tokenType.push_back("operator");
			    tokenValue.push_back(word);
			    continue;
            }
        }
        //decimal
        if(isDecimal(word) == true)
        {
            if(stringFlag == true)
            {
                tokenType.push_back("string");
			    tokenValue.push_back(word);
			    continue;
            }
            else
            {
                tokenType.push_back("decimal");
			    tokenValue.push_back(word);
			    continue;
            }
        }
        //nondecimal
        if(isNonDecimal(word) == true)
        {
            if(stringFlag == true)
            {
                tokenType.push_back("string");
			    tokenValue.push_back(word);
			    continue;
            }
            else
            {
                tokenType.push_back("nondecimal");
			    tokenValue.push_back(word);
			    continue;
            }
        }
    }
    source.close();
}
//Rule #1: An identifier must be declared to be used in any statement.
//return 1 on error
//check if statement contains identifier-if not then no need to continue
//if so, check all preceding statements for declaration/check if this statement is declaration itself
int ruleOne(int eosLocation)
{
    for(int i = statementCursor; i < eosLocation; i++)//loop moves through statement
    {
        if(tokenType[i] == "identifier")
        {
            //check all preceding and current statements for declaration
            for(int j = 0; j < eosLocation; j++)//moving from first word to current eos
            {
                //if there is a keyword followed by this identifier somewhere, its been declared
                if((tokenValue[j] == "integer" || tokenValue[j] == "decimal" || tokenValue[j] == "string" || tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i]))
                {
                    return 0;//been declared
                }
            }
            return 1;//not declared
        }
    }
    //will return from above if statement so if here, there was no identifier
    return 0;
}
//Rule #2: An identifier cannot be declared twice.
//return 1 on error
//check if statement contains "keyword -> identifier" -if not then no need to continue
//if so, check all preceding statements for identical declaration
int ruleTwo(int eosLocation)
{
    for(int i = statementCursor; i < eosLocation; i++)//loop moves through statement
    {
        if((tokenValue[i] == "integer" || tokenValue[i] == "decimal" || tokenValue[i] == "string" || tokenValue[i] == "bool") && tokenType[i + 1] == "identifier")//found declaration
        {
            //check all preceding statements for identical declaration
            for(int j = 0; j < statementCursor; j++)//moving from first word to current eos
            {
                if((tokenValue[j] == "integer" || tokenValue[j] == "decimal" || tokenValue[j] == "string" || tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i + 1]))
                {
                    return 1;//found identical declaration
                }
            }
        }
    }
    return 0;
}
//Rule #3: When an identifier is assigned to a value, their datatypes must match.
//Look for "identifier =" in statement, if so, make sure assigned value is equal to keyword type
int ruleThree(int eosLocation)
{
    for(int i = statementCursor; i < eosLocation; i++)
    {
        if(tokenType[i] == "identifier" && tokenValue[i + 1] == "=" && tokenType[i + 2] == "identifier")//assignment statement using identifiers-ignore for now
        {
            return 0;
        }
        if(tokenType[i] == "identifier" && tokenValue[i + 1] == "=" && tokenValue[i + 3] == "==")//assignment statement using logical-ignore for now
        {
            return 0;
        }
        if(tokenType[i] == "identifier" && tokenValue[i + 1] == "=")//"identifer ="
        {
            //find where the identifier was declared
            for(int j = 0; j < eosLocation; j++)//moving from first word to current eos
            {
                if((tokenValue[j] == "integer" || tokenValue[j] == "decimal" || tokenValue[j] == "string" || tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i]))//found declaration
                {
                    //check datatypes based on keyword
                    if(tokenValue[j] == "integer")
                    {
                        if(tokenType[i + 2] != "nondecimal")
                        {
                            return 1;//integer must be assigned nondecimal
                        }
                    }
                    if(tokenValue[j] == "decimal")
                    {
                        if(tokenType[i + 2] != "decimal")
                        {
                            return 1;//integer must be assigned nondecimal
                        }
                    }
                    if(tokenValue[j] == "string")
                    {
                        if(tokenType[i + 2] != "string")
                        {
                            return 1;//integer must be assigned nondecimal
                        }
                    }
                    if(tokenValue[j] == "bool")
                    {
                        if(tokenType[i + 2] != "boolean")
                        {
                            return 1;//integer must be assigned nondecimal
                        }
                    }
                }
            }
        }
    }
    return 0;
}
//Rule #4: When an identifier is assigned to an arithmetic, if both numbers are integers, then the destination id must be an integer, otherwise it is a decimal.
//return 1 if error
//Look for "identifier = identifer/number + / - / * / \ identifier/number"
//Once that statement is found, look through statements including this one to find the datatype of the result identifier, saving when found
//Compare that datatype to those in arithmetic to check for error
int ruleFour(int eosLocation)
{
	string firstType, secondType;
    for(int i = statementCursor; i < eosLocation; i++)//loop moves through statement
    {
        if((tokenType[i] == "identifier") && (tokenValue[i + 1] == "=") && (tokenType[i + 2] == "identifier" || tokenType[i + 2] == "decimal" || tokenType[i + 2] == "nondecimal") && (tokenValue[i + 3] == "+" || tokenValue[i + 3] == "-" || tokenValue[i + 3] == "*" || tokenValue[i + 3] == "\\") && (tokenType[i + 4] == "identifier" || tokenType[i + 4] == "decimal" || tokenType[i + 4] == "nondecimal"))
        {
            //first, check if either are identifiers because we will have to find what it was declared as and store them
            if(tokenType[i + 2] == "identifier")
            {
                //find datatype of this identifier
                for(int j = 0; j < eosLocation; j++)//moving from first word to current eos
                {
                    if((tokenValue[j] == "integer" || tokenValue[j] == "decimal" || tokenValue[j] == "string" || tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i + 2]))//found declaration
                    {
                        firstType = tokenValue[j];//storing first datatype for reference
                    }
                }
            }
            if(tokenType[i + 4] == "identifier")
            {
                //find datatype of this identifier
                for(int j = 0; j < eosLocation; j++)//moving from first word to current eos
                {
                    if((tokenValue[j] == "integer" || tokenValue[j] == "decimal" || tokenValue[j] == "string" || tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i + 4]))//found declaration
                    {
                        secondType = tokenValue[j];//storing second datatype for reference
                    }
                }
            }
            //now that we know whether any of our arithmetic is an id and have found their datatypes-we can find the datatype of the result and compare it for errors
            //iterate up to-including this statement to find where the result id is declared
            for(int j = 0; j < eosLocation; j++)//moving from first word to current eos
            {
                if((tokenValue[j] == "integer" || tokenValue[j] == "decimal" || tokenValue[j] == "string" || tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i]))//found declaration
                {
                    if((tokenType[i + 2] != "identifier") && (tokenType[i + 4] != "identifier"))//neither are identifiers
                    {
                        if(tokenValue[j] == "integer")//if result == integer - make sure arithmetic are both ints
                        {
                            if((tokenType[i + 2] != "nondecimal") || (tokenType[i + 4] != "nondecimal"))
                            {
                                return 1;//both arithmetic must be ints
                            }
                        }  
                        if(tokenValue[j] == "decimal")//if result == decimal - make sure at least one arithmetic is decimal
                        {
                            if((tokenType[i + 2] != "decimal") && (tokenType[i + 4] != "decimal"))
                            {
                                return 1;//assigning no decimals to an int result
                            }
                        }
                        if(tokenValue[j] == "string")//must be adding two strings to string result
                        {
                            if((tokenType[i + 2] != "string") || (tokenType[i + 4] != "string"))
                            {
                                return 1;//assigning a non string to string
                            }
                        }
                    }
                    else//one or both of arithmetic are identifiers
                    {
                        if(tokenValue[j] == "integer")//if result == integer - make sure arithmetic are both ints
                        {
                            if(tokenType[i + 2] == "identifier")//first is id
                            {
                                if(firstType != "integer")
                                {
                                    return 1;//must be int
                                }
                            }
                            else//not an id so we can check its type directly
                            {
                                if(tokenType[i + 2] != "nondecimal")
                                {
                                    return 1;//must be int
                                }
                            }
                            if(tokenType[i + 4] == "identifier")//second is identifier
                            {
                                if(secondType != "integer")
                                {
                                    return 1;//must be int
                                }
                            }
                            else//not an id so we can check its type directly
                            {
                                if(tokenType[i + 4] != "nondecimal")
                                {
                                    return 1;//must be int
                                }
                            }
                        }
                        if(tokenValue[j] == "decimal")//if result == decimal - make sure at least one arithmetic is decimal
                        {
                            if(tokenType[i + 2] == "identifier")//first is id
                            {
                                if(firstType != "decimal")
                                {
                                    if(secondType != "decimal")
                                    {
                                        return 1;//assigning no decimals to a decimal result
                                    }
                                }
                            }
                            else//not an id so we can check its type directly
                            {
                                if(tokenType[i + 2] != "decimal")
                                {
                                    if(tokenType[i + 4] != "decimal")
                                    {
                                        return 1;//assigning no decimals to a decimal result
                                    }
                                }
                            }
                            if(tokenType[i + 4] == "identifier")//second is identifier
                            {
                                if(secondType != "decimal")
                                {
                                    if(firstType != "decimal")
                                    {
                                        return 1;//assigning no decimals to a decimal result
                                    }
                                }
                            }
                            else//not an id so we can check its type directly
                            {
                                if(tokenType[i + 4] != "decimal")
                                {
                                    if(tokenType[i + 2] != "decimal")
                                    {
                                        return 1;//assigning no decimals to decimal result
                                    }
                                }
                            }   
                        }
                        if(tokenValue[j] == "string")//must be adding two strings to string result
                        {
                            if(tokenType[i + 2] == "identifier")//first is id
                            {
                                if(firstType != "string")
                                {
                                    return 1;//must be string
                                }
                            }
                            else//not an id so we can check its type directly
                            {
                                if(tokenType[i + 2] != "string")
                                {
                                    return 1;//must be string
                                }
                            }
                            if(tokenType[i + 4] == "identifier")//second is identifier
                            {
                                if(secondType != "string")
                                {
                                    return 1;//must be string
                                }
                            }
                            else//not an id so we can check its type directly
                            {
                                if(tokenType[i + 4] != "string")
                                {
                                    return 1;//must be string
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
//Rule #5: true and false can only be assigned to bool variables. (already covered in rule 3) Only + & * allowed with Booleans
//return 1 if error
//Look for a "identifer operator identifier" - if one is boolean the other must be or error - if both boolean and operator is not + or *; error
int ruleFive(int eosLocation)
{
    bool bFlag = false;
    for(int i = statementCursor; i < eosLocation; i++)//loop moves through statement
    {
        if((tokenType[i] == "identifier") && (tokenType[i + 1] == "operator") && (tokenType[i + 2] == "identifier"))
        {
            //check to see if one of ID's is boolean and the other isnt - error
            //if both are booleans - check if operator is + or * - if not, error
            for(int j = 0; j < eosLocation; j++)//first, finding declaration of first identifier
            {
                if((tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i]))//looking for bool declaration of first id
                {
                    bFlag = true;//its declared as bool;
                }
            }
            for(int k = 0; k < eosLocation; k++)//now, finding declaration of second identifier
            {
                if((tokenValue[k] == "string" || tokenValue[k] == "decimal" || tokenValue[k] == "integer") && (tokenValue[k + 1] == tokenValue[i + 2]))//looking for non bool declaration of second id
                {
                    if(bFlag == true)
                    {
                        return 1;//cant use operators with a boolean and non boolean
                    }
                }
                if((tokenValue[k] == "bool") && (tokenValue[k + 1] == tokenValue[i + 2]))//if second declaration of id is bool, check if valid operator
                {
                    if(tokenValue[i + 1] != "+" && tokenValue[i + 1] != "*" && tokenValue[i + 1] != "=")
                    {
                        cout << tokenValue[i + 1] << endl;
                        return 1;
                    }
                }
            }
            
        }
        
    }
    return 0;
}
//Rule #6: When assigning an id to <logical> the datatype of the id must be bool.
//<logical> (<id> | <value>) == (<id> | <value>)
//look for "id = (id | decimal | nondecimal | string | boolean) == (id | decimal | nondecimal | string | boolean)"
//Then, make sure the first ID is bool when declared
int ruleSix(int eosLocation)
{
    for(int i = statementCursor; i < eosLocation; i++)//loop moves through statement
    {
        if((tokenType[i] == "identifier") && (tokenValue[i + 1] == "=") && (tokenType[i + 2] == "identifier" || tokenType[i + 2] == "decimal" || tokenType[i + 2] == "nondecimal" || tokenType[i + 2] == "string" || tokenType[i + 2] == "boolean") && (tokenValue[i + 3] == "==") && (tokenType[i + 4] == "identifier" || tokenType[i + 4] == "decimal" || tokenType[i + 4] == "nondecimal" || tokenType[i + 4] == "string" || tokenType[i + 4] == "boolean"))//found id assigned to logical
        {
            for(int j = 0; j < eosLocation; j++)//first, finding declaration of first identifier
            {
                if((tokenValue[j] == "string" || tokenValue[j] == "integer" || tokenValue[j] == "decimal") && (tokenValue[j + 1] == tokenValue[i]))//found non bool declaration of first id
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
//Rule #7: The only operator that can be used with strings in <arithmetic> is +.
//look for id operator id
//if both id's are strings - make sure operator is + - or else error
int ruleSeven(int eosLocation)
{
    for(int i = statementCursor; i < eosLocation; i++)//loop moves through statement
    {
        if((tokenType[i] == "identifier") && (tokenType[i + 1] == "operator") && (tokenType[i + 2] == "identifier"))//found "id op id"
        {
            for(int j = 0; j < eosLocation; j++)//first, finding string declaration of first identifier
            {
                if(tokenValue[j] == "string" && tokenValue[j + 1] == tokenValue[i])//found string declaration
                {
                    for(int k = 0; k < eosLocation; k++)//now finding string declaration of second identifier
                    {
                        if(tokenValue[k] == "string" && tokenValue[k + 1] == tokenValue[i + 2])//found string declaration
                        {
                            //both are strings so must check if operator is '+'
                            if(tokenValue[i + 1] != "+")
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
//Rule #8: Input statements can only be used to get numbers and/or strings - cannot read bool
//Look for "read id"
//Make sure that id is integer, decimal, or string
int ruleEight(int eosLocation)
{
	for(int i = statementCursor; i < eosLocation; i++)//loop moves through statement
	{
		if((tokenValue[i] == "read") && (tokenType[i + 1] == "identifier"))
		{
			for(int j = 0; j < eosLocation; j++)//finding bool declaration of identifier
			{
				if((tokenValue[j] == "bool") && (tokenValue[j + 1] == tokenValue[i + 1]))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int main()
{
    string file;
    cout << "Enter filename: ";
    cin >> file;
    lexer(file);
    //below for debugging 
    /*int field_one_width = 0, field_two_width = 0;
    for(int i = 0; i < tokenType.size(); ++i)
    {
        if(tokenType[i].length() > field_one_width)
        {
            field_one_width = tokenType[i].length();
        }
        if(tokenValue[i].length() > field_two_width)
        {
            field_two_width = tokenValue[i].length();
        }
    }
    cout<<setw(field_one_width)<<left<<"Token Type";
    cout<<"          ";
    cout<<setw(field_two_width)<<left<<"Token Value" <<"\n";
    for(int i = 0; i < tokenType.size(); ++i)
    {
        cout<<setw(field_one_width)<<left<<tokenType[i];
        cout<<"          ";
        cout<<setw(field_two_width)<<left<<tokenValue[i] <<"\n";
    }
    cout << endl;*/
    //begin parsing
    for(int i = 0; i < tokenType.size(); i++)
    {
        if(tokenType[i] == "eos")
        {
            errorCount++;//keeps track of statement-used to return the error the line occured on
            //give eos location to rule functions-which check statements & return error if needed
            if(ruleOne(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": undeclared identifier" << endl;
                return 0;
            }
            if(ruleTwo(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": identifier has already been declared" << endl;
                return 0;
            }
            if(ruleThree(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": datatype does not match identifier" << endl;
                return 0;
            }
            if(ruleFour(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": result of an assignment statement must match arithmetic datatypes" << endl;
                return 0;
            }
            if(ruleFive(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": only + or * can be used as operators of booleans" << endl;
                return 0;
            }
            if(ruleSix(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": identifier assigned to logical statement must be of type bool" << endl;
                return 0;
            }
            if(ruleSeven(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": only + can be used as an operator of strings" << endl;
                return 0;
            }
            if(ruleSeven(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": only + can be used as an operator of strings" << endl;
                return 0;
            }
            if(ruleEight(i) == 1)
            {
                cout << "Error in statement " << errorCount << ": cannot read in bool " << endl;
                return 0;
            }
            statementCursor = i;//update before looping to next eos
        }
        
    }
   cout << "Successfully compiled." << endl;
    return 0;
}