#!/usr/bin/env python3
"""
RISC-V Memory File Converter

Converts .mem files from half-word hex format to full-word little-endian format.
Each pair of input half-words is combined into a 32-bit word and converted to little-endian.
"""

import sys
import os

def convert_mem_file(input_file, output_file):
    """
    Convert RISC-V memory file from half-word hex to full-word little-endian format.
    
    Args:
        input_file (str): Path to input .mem file
        output_file (str): Path to output .mem file
    """
    try:
        # Read input file
        with open(input_file, 'r') as f:
            lines = [line.strip() for line in f.readlines() if line.strip()]
        
        # Ensure we have an even number of half-words
        if len(lines) % 2 != 0:
            raise ValueError(f"Input file must contain an even number of half-words. Found {len(lines)} lines.")
        
        # Process pairs of half-words
        output_words = []
        for i in range(0, len(lines), 2):
            half_word1 = lines[i]     # Lower half-word
            half_word2 = lines[i + 1] # Upper half-word
            
            # Validate hex format
            if not all(c in '0123456789abcdefABCDEF' for c in half_word1):
                raise ValueError(f"Invalid hex format in line {i+1}: {half_word1}")
            if not all(c in '0123456789abcdefABCDEF' for c in half_word2):
                raise ValueError(f"Invalid hex format in line {i+2}: {half_word2}")
            
            # Ensure 4-character format
            half_word1 = half_word1.zfill(4).upper()
            half_word2 = half_word2.zfill(4).upper()
            
            # Combine into 32-bit word (upper << 16 | lower)
            combined_word = half_word2 + half_word1
            
            # Convert to integer
            word_int = int(combined_word, 16)
            
            # Convert to little-endian format
            byte0 = (word_int & 0xFF)
            byte1 = (word_int >> 8) & 0xFF
            byte2 = (word_int >> 16) & 0xFF
            byte3 = (word_int >> 24) & 0xFF
            
            # Format as little-endian hex string
            little_endian_word = f"{byte0:02x}{byte1:02x}{byte2:02x}{byte3:02x}".upper()
            output_words.append(little_endian_word)
        
        # Write output file
        with open(output_file, 'w') as f:
            for word in output_words:
                f.write(word + '\n')
        
        print(f"Successfully converted {len(lines)} half-words to {len(output_words)} full words")
        print(f"Input: {input_file}")
        print(f"Output: {output_file}")
        
    except FileNotFoundError:
        print(f"Error: Input file '{input_file}' not found.")
        return False
    except ValueError as e:
        print(f"Error: {e}")
        return False
    except Exception as e:
        print(f"Unexpected error: {e}")
        return False
    
    return True

def main():
    """Main function - automatically processes in_text.mem to out_text.mem."""
    input_file = "in_text.mem"
    output_file = "out_text.mem"
    
    # Validate file extensions (optional)
    if not input_file.endswith('.mem'):
        print("Warning: Input file doesn't have .mem extension")
    if not output_file.endswith('.mem'):
        print("Warning: Output file doesn't have .mem extension")
    
    # Convert the file
    success = convert_mem_file(input_file, output_file)
    
    if success:
        print("\nConversion completed successfully!")
    else:
        print("\nConversion failed!")
        sys.exit(1)

# Example usage function for testing
def test_example():
    """Test the converter with the provided example."""
    test_input = """2403
0000
4413
6df4
2423
0080
006f
0000"""
    
    # Write test input file
    with open('test_input.mem', 'w') as f:
        f.write(test_input)
    
    # Convert
    if convert_mem_file('test_input.mem', 'test_output.mem'):
        # Read and display output
        with open('test_output.mem', 'r') as f:
            output = f.read().strip()
        print("\nTest output:")
        print(output)
        
        expected = """03240000
1344F46D
23248000
6F000000"""
        
        if output.upper() == expected.upper():
            print("\n✓ Test passed! Output matches expected result.")
        else:
            print("\n✗ Test failed! Output doesn't match expected result.")
            print("Expected:")
            print(expected)
    
    # Clean up test files
    try:
        os.remove('test_input.mem')
        os.remove('test_output.mem')
    except:
        pass

if __name__ == "__main__":
    # Uncomment the line below to run the test
    # test_example()
    
    main()
