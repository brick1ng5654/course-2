def load_file(filename):
    array = []
    try:
        with open(filename, 'r') as f:
            for line in f:
                array_line = line.strip().split(';')
                array.append(array_line)
    except FileNotFoundError:
        print(f"Файл {filename} не найден.")
    except Exception as e:
        print(f"Произошла ошибка при чтении файла: {e}")
    return array

if __name__ == "__main__":
    result = load_file('data.txt')
    print(result)