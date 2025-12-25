# DSA_projcet
# XML Editor - DSA Project

A comprehensive command-line XML processing tool that provides various operations for XML files including compression, formatting, conversion, error handling, network analysis, and search functionality.

## Features

### 1. **Compression & Decompression**
   - Compress XML files to reduce file size
   - Decompress compressed files back to XML format

### 2. **Formatting**
   - Prettify XML files with proper indentation and formatting
   - Makes XML files more readable

### 3. **Minifying**
   - Remove unnecessary whitespace and formatting
   - Create compact XML files

### 4. **XML to JSON Conversion**
   - Convert XML files to JSON format
   - Preserves data structure during conversion

### 5. **Error Handling & Verification**
   - Verify XML files for errors
   - Fix common XML errors automatically
   - Generate error-free output files

### 6. **Network Analysis**
   - Visualize social networks as graphs
   - Find most influential users
   - Identify most active users
   - Suggest followers for users
   - Find mutual connections between users

### 7. **Search Functionality**
   - Search for specific words in XML content
   - Search by topics
   - Returns posts/IDs where matches are found

## Project Structure

```
DSA_projcet-main/
├── compression/          # Compression and decompression modules
├── ErrorHandling/        # XML error detection and correction
├── Format/               # XML formatting and prettification
├── mini/                 # XML minification
├── Network_analysis/     # Social network analysis algorithms
├── Search/               # Search functionality
├── XmlToGraph/           # XML to graph conversion
├── XmlToJson/            # XML to JSON conversion
├── XMLParser.cpp         # XML parsing implementation
├── XMLParser.h           # XML parsing header
├── XMLTree.h             # XML tree data structure
├── generalHeader.h       # Common headers and utilities
└── main.cpp              # Main entry point
```

## Building the Project

### Compilation

```bash
# Using g++
g++ -std=c++11 -o xml_editor main.cpp XMLParser.cpp \
    compression/compress.cpp \
    Format/format.cpp \
    XmlTojson/XmlTojson.cpp \
    mini/mini.cpp \
    Network_analysis/network_anaysis.cpp \
    Search/Search.cpp \
    ErrorHandling/ErrorHandling.cpp \
    XmlToGraph/XmlToGraph.cpp

# Or compile all .cpp files in the project
g++ -std=c++11 -o xml_editor *.cpp */*.cpp
```

## Usage

The tool is used via command-line with the following syntax:

```bash
./xml_editor <command> [options]
```

### Commands

#### 1. Compress XML File
```bash
xml_editor compress -i <input_file.xml> -o <output_file.comp>
```

#### 2. Decompress File
```bash
./xml_editor decompress -i <input_file.comp> -o <output_file.xml>
```

#### 3. Format XML File
```bash
./xml_editor format -i <input_file.xml> -o <output_file.xml>
```

#### 4. Minify XML File 
```bash
./xml_editor mini -i <input_file.xml> -o <output_file.xml>
```

#### 5. Convert XML to JSON
```bash
./xml_editor json -i <input_file.xml> -o <output_file.json>
```

#### 6. Verify and Fix XML Errors
```bash
./xml_editor verify -i <input_file.xml> -o <output_file.xml>
```

#### 7. Draw Network Graph
```bash
./xml_editor draw -i <input_file.xml> -o <output_file.jpg>
```
*Note: Requires Graphviz for visualization*

#### 8. Suggest Followers
```bash
./xml_editor suggest -i <input_file.xml> -id <user_id>
```

#### 9. Find Most Influential Users
```bash
./xml_editor most_influencer -i <input_file.xml>
```

#### 10. Find Most Active Users
```bash
./xml_editor most_active -i <input_file.xml>
```

#### 11. Find Mutual Users
```bash
./xml_editor mutual -i <input_file.xml> -ids <id1,id2,id3>
```

#### 12. Search by Word
```bash
./xml_editor search -i <input_file.xml> -w <word>
```

#### 13. Search by Topic
```bash
./xml_editor search -i <input_file.xml> -t <topic>
```

## Examples

### Basic Operations
```bash
# Compress an XML file
./xml_editor compress -i input_file.xml -o compressed.comp

# Format an XML file
./xml_editor format -i messy.xml -o formatted.xml

# Convert XML to JSON
./xml_editor json -i data.xml -o data.json

# Minify XML
./xml_editor mini -i large.xml -o compact.xml
```

### Network Analysis
```bash
# Visualize social network
./xml_editor draw -i network.xml -o network_graph.jpg

# Get follower suggestions for user ID 1
./xml_editor suggest -i network.xml -id 1

# Find most influential users
./xml_editor most_influencer -i network.xml

# Find mutual connections between users 1, 2, and 3
./xml_editor mutual -i network.xml -ids 1,2,3
```

### Search Operations
```bash
# Search for a word
./xml_editor search -i data.xml -w "lorem"

# Search by topic
./xml_editor search -i data.xml -t "economy"
```

## Input/Output

- **Input**: XML files in standard format
- **Output**: 
  - XML files (formatted, minified, decompressed, verified)
  - JSON files (converted from XML)
  - Compressed files (.comp extension)
  - Graph visualizations (JPG format)
  - Console output (for search and analysis commands)

## Error Handling

The tool includes comprehensive error handling:
- File I/O errors are caught and reported
- XML parsing errors are detected and can be fixed
- Invalid command-line arguments are validated
- Missing required parameters are reported with usage instructions

## Dependencies

- Standard C++11 libraries
- Graphviz (optional, for graph visualization)



## Contributing

This is a DSA (Data Structures and Algorithms) project. Contributions and improvements are welcome!

## Notes

- The project uses a custom XML parser implementation
- Network analysis features are designed for social network XML data
- Graph visualization requires Graphviz to be installed on the system
- All file operations preserve the original input files
