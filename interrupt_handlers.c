/* Interrupt Handlers */
void keyboard_vector(void);

void interrupt_vector(unsigned pc) {
    keyboard_vector();
    
  // Call gprof_vector as well if doing extension.
}

// The rest of these should never be called
void fast_interrupt_vector(int pc) {}
void reset_vector(int pc) {}
void undefined_instruction_vector(int pc) {}
void software_interrupt_vector(int pc) {}
void prefetch_abort_vector(int pc) {}
void data_abort_vector(int pc) {}
