def add_1_to_lines(file_name):
    try:
        with open(file_name, 'r') as file:
            lines = file.readlines()

        with open(file_name, 'w') as file:
            for line in lines:
                modified_line = line.strip() + ' 1\n'
                file.write(modified_line)

        print(f'Successfully modified {file_name}')
    except FileNotFoundError:
        print(f'Error: {file_name} not found')
    except Exception as e:
        print(f'An error occurred: {e}')

file_name = "rmat_large.txt"
add_1_to_lines(file_name)