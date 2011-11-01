static void TextEdit_change(TextEdit *self) {
  if(self->p.locked == false && self->onChange) self->onChange();
}

void pTextEdit::setCursorPosition(unsigned position) {
  GtkTextMark *mark = gtk_text_buffer_get_mark(textBuffer, "insert");
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  gtk_text_iter_set_offset(&iter, min(position, gtk_text_iter_get_offset(&iter)));
  gtk_text_buffer_place_cursor(textBuffer, &iter);
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(subWidget), mark);
}

void pTextEdit::setEditable(bool editable) {
  gtk_text_view_set_editable(GTK_TEXT_VIEW(subWidget), editable);
}

void pTextEdit::setText(const string &text) {
  locked = true;
  gtk_text_buffer_set_text(textBuffer, text, -1);
  locked = false;
}

void pTextEdit::selectAll() {
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(textBuffer, &start);
  gtk_text_buffer_get_end_iter(textBuffer, &end);
  gtk_text_buffer_select_range(textBuffer, &start, &end);
}

void pTextEdit::copyAll() {
  selectAll();
  gtk_text_buffer_copy_clipboard(textBuffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
}

void pTextEdit::setWordWrap(bool wordWrap) {
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), wordWrap ? GTK_WRAP_WORD_CHAR : GTK_WRAP_NONE);
}

string pTextEdit::text() {
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(textBuffer, &start);
  gtk_text_buffer_get_end_iter(textBuffer, &end);
  char *temp = gtk_text_buffer_get_text(textBuffer, &start, &end, true);
  string text = temp;
  g_free(temp);
  return text;
}

void pTextEdit::constructor() {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(gtkWidget), GTK_SHADOW_ETCHED_IN);
  subWidget = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), GTK_WRAP_WORD_CHAR);
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);
  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));
  g_signal_connect_swapped(G_OBJECT(textBuffer), "changed", G_CALLBACK(TextEdit_change), (gpointer)&textEdit);
  gtk_widget_show(subWidget);
}
