#include "FontLoader.h"
#include "Font.h"
#include "Resources/Model3d.h"

template<class T, class S>
void readObject(T& to_read, S& in)
{
  in.read(reinterpret_cast<char*>(&to_read), sizeof(T));
}

Resource* FontLoader::loadInternal(State* state, string dir)
{
  std::ifstream input(dir, std::ios::binary);
  if (input.fail() || input.get() != 'F' || input.get() != '0')
    throw std::runtime_error("Not a valid font file.");
  cout << "Loading valid font file\n";
  // Get the texture size, the number of glyphs and the line height.
  int line_height;
  float tex_line_height;
  GLuint texture;
  map<char,Font::Glyph> table;
  
  size_t width, height, n_chars;
  readObject(width, input);
  readObject(height, input);
  readObject(line_height, input);
  readObject(n_chars, input);
  tex_line_height = static_cast<float>(line_height) / height;
 
  // Read every glyph, store it in the glyph array and set the right
  // pointer in the table.
  
  struct Glyph_Buffer
  {
    unsigned char ascii, width;
    unsigned short x, y;
  };

  Glyph_Buffer buffer;
  for (size_t i = 0; i < n_chars; ++i){
    readObject(buffer, input);
    Font::Glyph tmp;
    float tex_x1 = static_cast<float>(buffer.x) / width;
    float tex_x2 = static_cast<float>(buffer.x + buffer.width) / width;
    float tex_y1 = static_cast<float>(buffer.y) / height;
    float tex_y2= tex_y1+tex_line_height;
    
    Vertex Vertices[4] = {
    Vertex(Vector3(.5f, -0.5f, 0.0f), Vector2(tex_x2, tex_y2), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(.5f, 0.5f, 0.0f), Vector2(tex_x2, tex_y1), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector2(tex_x1, tex_y1), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(-.5f, -0.5f, 0.0f), Vector2(tex_x1, tex_y2), Vector3(0.0f,1.0f,0.0f)) };
    
    glGenBuffers(1, &tmp.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, tmp.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    tmp.advance = buffer.width;
 
    table.insert(pair<char,Font::Glyph>(buffer.ascii,tmp));
  }
  cout << "loaded: " << table.size() << " fonts" << endl;
  // We must have the default character (stored under '\xFF')
  if (table.find((char)'\xFF') == table.end())
    throw std::runtime_error("Font file contains no default glyph");
 
  // Store the actual texture in an array.
  unsigned char* tex_data = new unsigned char[width * height];
  input.read(reinterpret_cast<char*>(tex_data), width * height);
 
  // Generate an alpha texture with it.
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, width, height, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, tex_data);
  // And delete the texture memory block
  delete[] tex_data;
  return new Font(dir, table, texture, line_height, tex_line_height);
}
