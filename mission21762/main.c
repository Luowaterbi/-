#include "PickupLeftFactor.h"
#include <string.h>
#include <stdlib.h>

const char* VoidSymbol = "$"; // "��"
const char* Postfix = "'";

char rule_table_ci[20][256];
char ruleNameArr[20][64];

int main(int argc, char* argv[])
{
	//
	// ���� InitRules ������ʼ���ķ�
	//
#ifdef CODECODE_CI
	Rule* pHead = InitRules_CI();  	// ���д�����������ˮ������
#else
	Rule* pHead = InitRules();		// ���д����� CP Lab ������
#endif

	//
	// �����ȡ������֮ǰ���ķ�
	//
	printf("Before Pickup Left Factor:\n");
	PrintRule(pHead);

	//
	// ���� PickupLeftFactor �������ķ���ȡ������
	//
	PickupLeftFactor(pHead);
	
	//
	// �����ȡ������֮����ķ�
	//
	printf("\nAfter Pickup Left Factor:\n");
	PrintRule(pHead);
	
	FreeRule(pHead);
	return 0;
}

/*
���ܣ�
	�����±��ҵ� Select �е�һ�� Symbol��

������
	pSelect -- Select ָ�롣
	index -- �±ꡣ
	  
����ֵ��
	������ڣ������ҵ��� Symbol ָ�룬���򷵻� NULL��
*/
RuleSymbol* GetSymbol(RuleSymbol* pSelect, int index)
{
	int i = 0;
	RuleSymbol* pRuleSymbol;
	for (pRuleSymbol = pSelect, i = 0; pRuleSymbol != NULL; 
		pRuleSymbol = pRuleSymbol->pNextSymbol, i++)
	{
		if (i == index)
		{
			return pRuleSymbol;
		}
	}

	return NULL;	
}

/*
���ܣ�
	�� SelectTemplate Ϊģ�壬ȷ�������ӵ���󳤶ȡ�

������
	pSelectTemplate -- ��Ϊģ��� Select ָ�롣
	  
����ֵ��
	�����ӵ���󳤶ȣ�������� 0 ˵�������������ӡ�
*/
int LeftFactorMaxLength(RuleSymbol* pSelectTemplate)
{
	RuleSymbol *pCurSelect=pSelectTemplate->pOther;
	int Count=MAX_STR_LENGTH;
	while(pCurSelect){
		int TmpCount=0;
		while(SymbolCmp(GetSymbol(pSelectTemplate,TmpCount),GetSymbol(pCurSelect,TmpCount))) 
			++TmpCount;
		if(TmpCount<Count) Count=TmpCount;
		pCurSelect=pCurSelect->pOther;
	}
	return Count==MAX_STR_LENGTH?0:Count;
}

/*
���ܣ�
	�Ƚ�������ͬ����(ͬΪ�ս����ͬΪ���ս��)�� Symbol �Ƿ������ͬ�����֡�

������
	pSymbol1 -- Symbol ָ�롣
	pSymbol2 -- Symbol ָ�롣
	  
����ֵ��
	��ͬ���� 1����ͬ���� 0��
*/
int SymbolCmp(RuleSymbol* pSymbol1, RuleSymbol* pSymbol2)
{

	if(pSymbol1->isToken==1&&pSymbol2->isToken==1) return strcmp(pSymbol1->TokenName,pSymbol2->TokenName)==0;
	return (pSymbol1->pRule==pSymbol2->pRule);
	
}

/*
���ܣ�
	ȡ�ķ��е�һ�� Select �� SelectTemplate ���бȽϣ��жϸ� Select �Ƿ���Ҫ��ȡ�����ӡ�

������
	pSelectTemplate -- ��Ϊģ��� Select ָ�롣
	Count -- SelectTemplate ����ȷ���������ӵ�������
	pSelect -- Select ָ�롣
	  
����ֵ��
	��� Select ���������ӷ��� 1�����򷵻� 0��
*/
int NeedPickup(RuleSymbol* pSelectTemplate, int Count, RuleSymbol* pSelect)
{
	for(int i=0;i<Count;i++)
		if(!SymbolCmp(GetSymbol(pSelectTemplate,i),GetSymbol(pSelect,i))) return 0;
	return 1;
	
}

/*
���ܣ�
	��һ�� Select ���뵽�ķ�ĩβ���� Select Ϊ NULL ʱ�ͽ�һ�����ս�����뵽�ķ�ĩβ��

������
	pRule -- �ķ�ָ�롣
	pNewSelect -- Select ָ�롣
*/
void AddSelectToRule(Rule* pRule, RuleSymbol* pNewSelect)
{

	RuleSymbol** pTmpSelect=&(pRule->pFirstSymbol);
    while((*pTmpSelect)!=NULL) pTmpSelect=&((*pTmpSelect)->pOther);
    (*pTmpSelect)=pNewSelect;
    return ;
}

/*
���ܣ�
	�� pRuleName ���ķ��е����� RuleName �Ƚ�, �����ͬ������һ����׺��

������
	pHead -- Rule ������ͷָ�롣
	pRuleName -- Rule �����֡�
*/
void GetUniqueRuleName(Rule* pHead, char* pRuleName)
{
	Rule* pRuleCursor = pHead;
	for (; pRuleCursor != NULL;)
	{
		if (0 == strcmp(pRuleCursor->RuleName, pRuleName))
		{
			strcat(pRuleName, Postfix);
			pRuleCursor = pHead;
			continue;
		}
		pRuleCursor = pRuleCursor->pNextRule;
	}	
}

/*
���ܣ�
	�ͷ�һ�� Select ���ڴ档

������
	pSelect -- ��Ҫ�ͷŵ� Select ��ָ�롣
*/
void FreeSelect(RuleSymbol* pSelect)
{
	if(pSelect==NULL) return ;
    FreeSelect(pSelect->pNextSymbol);
	free(pSelect);
    return ;
}

/*
���ܣ�
	�ͷ����� Rule ���ڴ档

������
	pRule -- ���� Rule ��ͷָ�롣
*/
void FreeRule(Rule* pRule)
{
	if(pRule==NULL) return ;
    FreeRule(pRule->pNextRule);
	RuleSymbol *pCurSelect=pRule->pFirstSymbol,*pNxtSelect;
	while(pCurSelect){
		pNxtSelect=pCurSelect->pOther;
		FreeSelect(pCurSelect);
		pCurSelect=pNxtSelect;
	}
	free(pRule);
    return ;
}

/*
���ܣ�
	��ȡ�����ӡ�

������
	pHead -- �ķ���ͷָ�롣
*/
void PickupLeftFactor(Rule* pHead)
{
	Rule* pRule;		    	 // Rule �α�
	int isChange;				 // Rule �Ƿ���ȡ�����ӵı�־
	RuleSymbol* pSelectTemplate; // Select �α�
	Rule* pNewRule; 			 // Rule ָ��
	RuleSymbol* pSelect;		 // Select �α�
	
	do
	{
		isChange = 0;

		for(pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
		{
			// ȡ Rule �е�һ�� Select ��Ϊģ�壬���� LeftFactorMaxLength ����ȷ�������ӵ���󳤶�
			int Count = 0;
			for(pSelectTemplate = pRule->pFirstSymbol; pSelectTemplate != NULL; pSelectTemplate = pSelectTemplate->pOther)
			{
				if((Count = LeftFactorMaxLength(pSelectTemplate)) > 0)
					break;
			}

			// ����û�������ӵ� Rule
			if(Count == 0)
				continue;

			pNewRule = CreateRule(pRule->RuleName); // ������ Rule
			GetUniqueRuleName(pRule, pNewRule->RuleName);
			isChange = 1; // ���ñ�־

			// ���� AddSelectToRule ������ģ��������֮��Ĳ��ּӵ��� Rule ��ĩβ
			// ��ģ��������֮��Ĳ����滻Ϊָ���� Rule �ķ��ս��
			
			RuleSymbol* pTmpSymbol=GetSymbol(pSelectTemplate,Count-1);
			AddSelectToRule(pNewRule,pTmpSymbol->pNextSymbol);
			RuleSymbol *pNewSymbol=CreateSymbol();
			pNewSymbol->isToken=0;
			pNewSymbol->pRule=pNewRule;
			pTmpSymbol->pNextSymbol=pNewSymbol;
			

			// ��ģ��֮���λ��ѭ�����Ұ��������ӵ� Select������ȡ������
			pSelect = pSelectTemplate->pOther;
			RuleSymbol **pSelectPtr = &pSelectTemplate->pOther;
			while(pSelect != NULL)
			{
				if(NeedPickup(pSelectTemplate, Count, pSelect)) // Select ����������
				{
					// ���� AddSelectToRule ������������֮��Ĳ��ּӵ��� Rule ��ĩβ
					// ���� Select �� Rule ���Ƴ����ͷ��ڴ棬���ƶ��α�
					RuleSymbol* pTmpSymbol=GetSymbol(pSelect,Count-1);
					AddSelectToRule(pNewRule,pTmpSymbol->pNextSymbol);
					pTmpSymbol->pNextSymbol=NULL;
					RuleSymbol* pFreeSymbol=pSelect;
					pSelect=pSelect->pOther;
					(*pSelectPtr)=pSelect;
					FreeSelect(pFreeSymbol);
				}
				else // Select ������������
				{
					// �ƶ��α�
					pSelectPtr=&pSelect->pOther;
					pSelect=pSelect->pOther;
				}
			}

			// ���� Rule ���뵽�ķ�����
			
			Rule* pTmpRule=pRule;
			while(pTmpRule->pNextRule!=NULL) pTmpRule=pTmpRule->pNextRule;
			pTmpRule->pNextRule=pNewRule;
		}

	} while (isChange == 1);
}

/*
���ܣ�
	ʹ�ø��������ݳ�ʼ���ķ�����

����ֵ��
	�ķ���ͷָ��
*/
typedef struct _SYMBOL
{
	int isToken;
	char Name[MAX_STR_LENGTH];
}SYMBOL;

typedef struct _RULE_ENTRY
{
	char RuleName[MAX_STR_LENGTH];
	SYMBOL Selects[64][64];
}RULE_ENTRY;

static const RULE_ENTRY rule_table[] =
{
	/* A -> abC | abcD | abcE */
	{ "A", 
			{
				{ { 1, "a" }, { 1, "b" }, { 1, "C" } },
				{ { 1, "a" }, { 1, "b" }, { 1, "c" }, { 1, "D" } },
				{ { 1, "a" }, { 1, "b" }, { 1, "c" }, { 1, "E" } }
			}	
	}
};

/*
���ܣ�
	��ʼ���ķ�������
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules()
{
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr1, **pSymbolPtr2;
	int nRuleCount = sizeof(rule_table) / sizeof(rule_table[0]);
	int i, j, k;

	Rule** pRulePtr = &pHead;
	for (i=0; i<nRuleCount; i++)
	{
		*pRulePtr = CreateRule(rule_table[i].RuleName);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (i=0; i<nRuleCount; i++)
	{
		pSymbolPtr1 = &pRule->pFirstSymbol;
		for (j=0; rule_table[i].Selects[j][0].Name[0] != '\0'; j++)
		{
			pSymbolPtr2 = pSymbolPtr1;
			for (k=0; rule_table[i].Selects[j][k].Name[0] != '\0'; k++)
			{
				const SYMBOL* pSymbol = &rule_table[i].Selects[j][k];

				*pSymbolPtr2 = CreateSymbol();
				(*pSymbolPtr2)->isToken = pSymbol->isToken;
				if (1 == pSymbol->isToken)
				{
					strcpy((*pSymbolPtr2)->TokenName, pSymbol->Name);
				}
				else
				{
					(*pSymbolPtr2)->pRule = FindRule(pHead, pSymbol->Name);
					if (NULL == (*pSymbolPtr2)->pRule)
					{
						printf("Init rules error, miss rule \"%s\"\n", pSymbol->Name);
						exit(1);
					}
				}
				pSymbolPtr2 = &(*pSymbolPtr2)->pNextSymbol;
			}

			pSymbolPtr1 = &(*pSymbolPtr1)->pOther;
		}

		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
���ܣ�
	��ʼ���ķ�����(��ִ����ˮ��ʱ����)��
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules_CI()
{
	int nRuleCount = 0;
	for (int i = 0; i < 20; i++)
	{
		gets(rule_table_ci[i]);	
		int length = strlen(rule_table_ci[i]);
		if (length == 0)
		{
			break;
		}
		
		for (int j = 0; j < length; j++)
		{
			if (rule_table_ci[i][j] == ' ')
			{
				ruleNameArr[i][j] = '\0';
				break;
			}
			ruleNameArr[i][j]= rule_table_ci[i][j];
		}	  
		
		nRuleCount++;
	}
			
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr1, **pSymbolPtr2;
		
	int i, j, k;

	Rule** pRulePtr = &pHead;
	for (i=0; i<nRuleCount; i++)
	{
		*pRulePtr = CreateRule(ruleNameArr[i]);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (i=0; i<nRuleCount; i++)
	{
		pSymbolPtr1 = &pRule->pFirstSymbol;
		
		int start = 0;
		for (int j=0; rule_table_ci[i][j] != '\0'; j++)
		{
			if (rule_table_ci[i][j] == ' '
			 && rule_table_ci[i][j + 1] == '-'
			&& rule_table_ci[i][j + 2] == '>' 
			&& rule_table_ci[i][j + 3] == ' ')
			{
				start = j + 4;
				break;
			}
		}
			
		for (k = start; rule_table_ci[i][k] != '\0'; k++)
		{
			if (rule_table_ci[i][k] == '|')
			{
				pSymbolPtr1 = &(*pSymbolPtr1)->pOther;
				pSymbolPtr2 = pSymbolPtr1;
				continue;
			}
			if (rule_table_ci[i][k] == ' ')
			{
				continue;
			}
			if (k == start)
			{
				pSymbolPtr2 = pSymbolPtr1;
			}

			*pSymbolPtr2 = CreateSymbol();
			
			char tokenName[MAX_STR_LENGTH] = {};
			tokenName[0] = rule_table_ci[i][k];
			tokenName[1] = '\0';
			(*pSymbolPtr2)->isToken = 1;
			for (int m = 0; m < nRuleCount; m++)
			{
				if (strcmp(tokenName, ruleNameArr[m]) == 0)
				{
					(*pSymbolPtr2)->isToken = 0;
					(*pSymbolPtr2)->pRule = FindRule(pHead, tokenName);
					if (NULL == (*pSymbolPtr2)->pRule)
					{
						printf("Init rules error, miss rule \"%s\"\n", tokenName);
						exit(1);
					}
				}
			}
			if ((*pSymbolPtr2)->isToken == 1)
			{
				strcpy((*pSymbolPtr2)->TokenName, tokenName);
			}
			
			pSymbolPtr2 = &(*pSymbolPtr2)->pNextSymbol;
			
		}
			
		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
���ܣ�
	����һ���µ� Rule��

������
	pRuleName -- �ķ������֡�
	
����ֵ��
	Rule ָ��	
*/
Rule* CreateRule(const char* pRuleName)
{
	Rule* pRule = (Rule*)malloc(sizeof(Rule));

	strcpy(pRule->RuleName, pRuleName);
	pRule->pFirstSymbol = NULL;
	pRule->pNextRule = NULL;

	return pRule;
}

/*
���ܣ�
	����һ���µ� Symbol��
	
����ֵ��
	RuleSymbol ָ��	
*/
RuleSymbol* CreateSymbol()
{
	RuleSymbol* pSymbol = (RuleSymbol*)malloc(sizeof(RuleSymbol));

	pSymbol->pNextSymbol = NULL;
	pSymbol->pOther = NULL;
	pSymbol->isToken = -1;
	pSymbol->TokenName[0] = '\0';
	pSymbol->pRule = NULL;

	return pSymbol;
}

/*
���ܣ�
	���� RuleName ���ķ������в���������ͬ���ķ���

������
	pHead -- �ķ���ͷָ�롣
	RuleName -- �ķ������֡�
	
����ֵ��
	Rule ָ��	
*/
Rule* FindRule(Rule* pHead, const char* RuleName)
{
	Rule* pRule;
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		if (0 == strcmp(pRule->RuleName, RuleName))
		{
			break;
		}
	}

	return pRule;
}

/*
���ܣ�
	����ķ���

������
	pHead -- �ķ���ͷָ�롣
*/
void PrintRule(Rule* pHead)
{
	Rule* pTmpRule=pHead;
    while(pTmpRule!=NULL){
        printf("%s->",pTmpRule->RuleName);
        RuleSymbol *pTmpSelect=pTmpRule->pFirstSymbol;
        while(pTmpSelect!=NULL){
            RuleSymbol *pTmpSymbol=pTmpSelect;
            while(pTmpSymbol!=NULL){
                printf("%s",pTmpSymbol->isToken?pTmpSymbol->TokenName:pTmpSymbol->pRule->RuleName);
                pTmpSymbol=pTmpSymbol->pNextSymbol;
            }
            pTmpSelect=pTmpSelect->pOther;
            if(pTmpSelect) printf("%s","|");
        }
        printf("\n");
        pTmpRule=pTmpRule->pNextRule;
    }
	return ;
}