g++ insert_example_sql.cpp -o insert -lsqlite3
g++ retrive_example_sql.cpp -o retrive -lsqlite3
echo "Insert"
./insert
echo "Retrive"
./retrive
