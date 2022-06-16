For the DSN task:
Runtime environment: at least Visual Studio 2019


Experiment:
Change the value of MAX_LEARNING_COUNT to 300000, 600000, and 1200000 respectively to reproduce the results in the paper.
MAX_LEARNING_COUNT is in  GlobalVariable.h.

const int MAX_LEARNING_COUNT = 1200000;  // The number of episodes for training in one experiment (one run). L in the paper.
