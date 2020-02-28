#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* month_name(int n);
void month_day(int *year, int yearday, int *pmonth, int *pday);
int isYearLeap(int *year);

int main(int argc, char** argv){

	// month_day();

	int year = atoi(argv[1]);
	int day = atoi(argv[2]);
	int pday;
	int pmonth = 0;

	month_day(&year, day, &pmonth, &pday);

	printf("%s %d, %d\n", month_name(pmonth), pday, year);



	return 0;
}

void month_day(int *year, int yearday, int *pmonth, int *pday) {
	int month_days[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};

	int leap = isYearLeap(year);

	int difYear = 0;

	if ( yearday > 365 ) {
		// printf("More than 365 days! Converting extra days to years...\n");
		if( (leap && yearday % 366 == 0) || (yearday % 366 == 0) ) {
			printf("true\n");
			difYear = leap ? (yearday/366) - 1 : (yearday/365) - 1;
			yearday -= (difYear/4) + leap;
			yearday = leap ? 366 : 365;
		} else {
			printf("false\n");
			difYear = leap ? yearday/366 : yearday/365;
			*year += difYear;
			if (isYearLeap(year)){
				printf("the actual year is leap\n");
				// yearday -= 1;
			}
			// yearday -= (difYear/4) + leap + 1;
			yearday = leap ? yearday%366 : yearday%365;
		}
	}

	// printf("The value of difYear is: %d\n", difYear);
	// printf("the value of yearday is: %d\n", yearday);
	// printf("The value of leap is: %d\n", leap);

	// *year += difYear;

	// printf("difference of year: %d\n", difYear);
	// printf("days to add: %d\n", (difYear/4) + leap);
	// yearday -= (difYear%4);

	int i = 0;
	for( i = 0 ; i < sizeof(month_days[leap])/4 ; i ++ ) {
		if ( yearday > month_days[leap][i] ) {
			yearday = yearday - month_days[leap][i];
		}
		else {
			break;
		}
	}
	*pday = yearday;
	*pmonth = i + 1;
	// printf("the value of pday is: %d\n", *pday);
	// printf("The value of pmonth is: %d\n", *pmonth);
}

int isYearLeap(int* year) {
	return ((*year)%4 == 0 && *year%100 != 0) || *year%400 == 0;
}

char *month_name(int n) {

   static char *name[] = {
       "Illegal month",
       "January", "February", "March",
       "April", "May", "June",
       "July", "August", "September",
       "October", "November", "December"
   };

   return (n < 1 || n > 12) ? name[0] : name[n];
}
