clang -Xpreprocessor -fopenmp -lomp palindrome.c -o palindrome 
echo "Number of words in file:"
wc -l words_alpha.txt 
./palindrome words_alpha.txt results.txt 
