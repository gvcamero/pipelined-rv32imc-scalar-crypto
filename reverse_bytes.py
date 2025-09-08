def reverse_bytes_in_words(input_file, output_file):
    with open(input_file, "r") as f:
        lines = f.readlines()

    output_lines = []
    for line in lines:
        word = line.strip()
        if not word:
            continue  # skip empty lines
        # Ensure even number of hex digits
        if len(word) % 2 != 0:
            raise ValueError(f"Invalid hex word length: {word}")
        # Split into bytes (2 hex chars each), reverse, and join
        bytes_list = [word[i:i+2] for i in range(0, len(word), 2)]
        reversed_word = "".join(reversed(bytes_list))
        output_lines.append(reversed_word)

    with open(output_file, "w") as f:
        f.write("\n".join(output_lines))

if __name__ == "__main__":
    # Example usage
    reverse_bytes_in_words("input.mem", "output.mem")
