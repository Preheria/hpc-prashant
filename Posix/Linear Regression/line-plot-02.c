/****************************************************************************
  This program demonstrates the equation of line, which define y as a 
  function of slope(m), x, and intercept(c) with the vertical axis. 
  Redirect the output of this program and plot the results as a scatter
  graph in a spreadsheet. This version enables parameters to be set from
  the command line. First parameter is m. Second parameter is c. Use this
  program to experiment with varying m and c.

  Compile with:

    cc -o line-plot-02 line-plot-02.c 
  
  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
double line(double m, double x, double c) {
  double y = (m * x) + c;
  return y;
}

int main(int args, char *argv[]) {
  double slope = (double) atof(argv[1]);// obtains slope value at runtime through commandline
  double intercept = (double) atof(argv[2]);// obtains intercept value at runtime through commandline
  double x, y;
  int i;
  
  printf("x,y\n");
  for(i=0;i<200;i++) {
	x = (i / 10.0);
	y = line(slope, x, intercept);
	printf("%lf,%lf\n", x, y);
  }
  
  return 0;
}
