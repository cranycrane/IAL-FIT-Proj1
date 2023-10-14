/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char stackTop;

	while(!Stack_IsEmpty(stack)) {
		// ulozime znak ze zasobniku 
		Stack_Top(stack, &stackTop);
		// jestli zavorka, tak koncime
		if (stackTop == '(') {
			Stack_Pop(stack);
			return;
		}

		postfixExpression[*postfixExpressionLength] = stackTop;
		++*postfixExpressionLength;

		Stack_Pop(stack);
	}
	return;
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char stackTop;

	if (Stack_IsEmpty(stack)) {
		Stack_Push(stack, c);
		return;
	}

	Stack_Top(stack, &stackTop);

	if ((c == '*' || c == '/') && (stackTop == '+' || stackTop == '-')) {
		Stack_Push(stack, c);
		return;
	}
	else if (stackTop == '(') {
		Stack_Push(stack, c);
		return;
	}
	else {
		postfixExpression[*postfixExpressionLength] = stackTop;
		++*postfixExpressionLength;
		Stack_Pop(stack);

		doOperation(stack, c, postfixExpression, postfixExpressionLength);
	}

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	char *postFixExpression = (char *)malloc(sizeof(char)*MAX_LEN);
	unsigned postFixExpressionLength = 0;
	if (postFixExpression == NULL) return NULL;

	Stack *stack = (Stack*)malloc(sizeof(Stack));
	if (stack == NULL) {
		free(postFixExpression);
		return NULL;
	}

	Stack_Init(stack);

	for (int i = 0; infixExpression[i] != '\0'; i++) {
		if ((infixExpression[i] >= '0' && infixExpression[i] <= '9') ||
			(infixExpression[i] >= 'a' && infixExpression[i] <= 'z') ||
			(infixExpression[i] >= 'A' && infixExpression[i] <= 'Z')) {
				postFixExpression[postFixExpressionLength] = infixExpression[i];
				++postFixExpressionLength;
			}
		else if (infixExpression[i] == ')') {
			untilLeftPar(stack, postFixExpression, &postFixExpressionLength);
		}
		else if (infixExpression[i] == '(') {
			Stack_Push(stack, infixExpression[i]);
		}
		else if (infixExpression[i] == '=') {
			while (!Stack_IsEmpty(stack)) {
				Stack_Top(stack, &postFixExpression[postFixExpressionLength]);
				++postFixExpressionLength;
				Stack_Pop(stack);
			}
			postFixExpression[postFixExpressionLength] = '=';
			++postFixExpressionLength;
			break;
		}
		else if (infixExpression[i] == '+' || infixExpression[i] == '-' || infixExpression[i] == '*' || infixExpression[i] == '/'){
			doOperation(stack, infixExpression[i], postFixExpression, &postFixExpressionLength);
		}
	}

	Stack_Dispose(stack);
	free(stack);

	postFixExpression[postFixExpressionLength] = '\0';

	return postFixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

	char byte[4];
	byte[0] = (value >> 24) & 0xFF;
	byte[1] = (value >> 16) & 0xFF;
	byte[2] = (value >> 8) & 0xFF;
	byte[3] = value & 0xFF;

	//char *bytePtr = (char *)&value;
	for (unsigned i = 0; i < sizeof(int); i++) {
		Stack_Push(stack, byte[i]);
	}
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	*value = 0;

	char byte;

	for (unsigned i = 0; i < sizeof(int); i++) {
		Stack_Top(stack, &byte);

		*value |= ((int)byte & 0xFF) << (i * 8);

		Stack_Pop(stack);
	}

}

VariableValue *processOperand(char operand, VariableValue variableValues[], int variableValueCount) {
	for (int i = 0; i < variableValueCount; i++) {
		if (operand == variableValues[i].name) {
			return &variableValues[i];
		}
	}
	return NULL;
}

int processOperator(Stack *stack, char operator) {
	int firstOperand;
	int secondOperand;

	expr_value_pop(stack, &secondOperand);
	expr_value_pop(stack, &firstOperand);

	switch(operator) {
		case '+':
			return firstOperand + secondOperand;
			break;
		case '-':
			return firstOperand - secondOperand;
			break;
		case '*':
			return firstOperand * secondOperand;
			break;
		case '/':
			return firstOperand / secondOperand;
			break;
		default:
			fprintf(stderr, "Znak operace nenalezen");
	}

	return -1;
}

int countExpressionLength(char *expression) {
	int counter = 0;
	while(expression[counter] != '\0') counter++;
	return counter;
}

/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
	 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	char *postFixExpression;
	postFixExpression = infix2postfix(infixExpression);

	Stack *stack = (Stack*)malloc(sizeof(Stack));

	Stack_Init(stack);

	int result = 0;

	for (int i = 0; postFixExpression[i] != '\0'; i++) {
		if ((postFixExpression[i] >= '0' && postFixExpression[i] <= '9') ||
			(postFixExpression[i] >= 'a' && postFixExpression[i] <= 'z') ||
			(postFixExpression[i] >= 'A' && postFixExpression[i] <= 'Z')) {
				VariableValue *currentOperand = processOperand(postFixExpression[i], variableValues, variableValueCount);
				if (currentOperand == NULL) {
					fprintf(stderr, "CHYBA: Operand nenalezen");
					Stack_Dispose(stack);
					return false;
				}
				expr_value_push(stack, currentOperand->value);
			}
		else if (postFixExpression[i] == '+' || postFixExpression[i] == '-' || postFixExpression[i] == '*' || postFixExpression[i] == '/'){
			result = processOperator(stack, postFixExpression[i]);
			expr_value_push(stack, result);
		}
	}

	*value = result;
	Stack_Dispose(stack);
	free(stack);
	free(postFixExpression);

	return true;
}

/* Konec c204.c */
