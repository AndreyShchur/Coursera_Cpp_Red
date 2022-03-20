#include <string>
#include <list>
#include "test_runner.h"
using namespace std;

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor()
  : cur_pos_(main_text_.end())
  {}

  void Left()
  {
      if (cur_pos_ != main_text_.begin())
      {
          cur_pos_--;
      }
  }
  void Right()
  {
      if (cur_pos_ != main_text_.end())
      {
          cur_pos_++;
      }
  }

  void Insert(char token)
  {
      main_text_.insert(cur_pos_, token);
      //cur_pos_++;
  }

  void Cut(size_t tokens = 1)
  {
      buffer_.erase(buffer_.begin(), buffer_.end());
      auto dist = distance(cur_pos_, main_text_.end());
      if (dist > tokens)
      {
          auto start_pos = cur_pos_;
          advance(cur_pos_, tokens);
          buffer_.insert(buffer_.begin(), start_pos, cur_pos_);
          main_text_.erase(start_pos, cur_pos_);
      }
      else
      {
          buffer_.insert(buffer_.begin(), cur_pos_, main_text_.end());
          main_text_.erase(cur_pos_, main_text_.end());
          cur_pos_ = main_text_.end();
      }
  }

  void Copy(size_t tokens = 1)
  {
      buffer_.erase(buffer_.begin(), buffer_.end());
      if (distance(cur_pos_, main_text_.end()) > tokens)
      {
          auto end_pos = cur_pos_;
          advance(end_pos, tokens);
          buffer_.insert(buffer_.begin(), cur_pos_, end_pos);
      }
      else
      {
          buffer_.insert(buffer_.begin(), cur_pos_, main_text_.end());
      }
  }

  void Paste()
  {
      main_text_.insert(cur_pos_, buffer_.begin(), buffer_.end());
      //advance(cur_pos_, distance(buffer_.begin(), buffer_.end()));
  }

  string GetText() const
  {
      return string(main_text_.begin(), main_text_.end());
  }

private:
    list<char> main_text_;
    list<char> buffer_;
    list<char>::iterator cur_pos_;
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}