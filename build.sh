mold -run cmake --build out
valgrind --leak-check=yes ./out/email-sender