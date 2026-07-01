import tkinter as tk
from tkinter import messagebox
import subprocess
import time

# --- Standard WCA Color Palette ---
COLORS = {
    'U': '#FFFFFF',  # White
    'D': '#FFFF00',  # Yellow
    'R': '#FF0000',  # Red
    'L': '#FF8C00',  # Orange (DarkOrange for readability)
    'F': '#00FF00',  # Green
    'B': '#0000FF'   # Blue
}

class FlatCubeSolverGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("C++ Bitboard 2D Rubik's Solver")
        self.root.configure(bg='#222222')
        
        # Initialize an internal representation of an unfolded cube (9 stickers per face)
        # Faces are ordered: U, L, F, R, B, D
        self.state = {
            'U': [['U']*3 for _ in range(3)],
            'L': [['L']*3 for _ in range(3)],
            'F': [['F']*3 for _ in range(3)],
            'R': [['R']*3 for _ in range(3)],
            'B': [['B']*3 for _ in range(3)],
            'D': [['D']*3 for _ in range(3)],
        }
        
        # Define layout offsets for the 2D cross representation on the canvas
        # (row_offset, col_offset) in terms of 3x3 grids
        self.face_offsets = {
            'U': (0, 1),
            'L': (1, 0), 'F': (1, 1), 'R': (1, 2), 'B': (1, 3),
            'D': (2, 1)
        }
        
        self.sticker_size = 30
        self.grid_gap = 5
        
        # Create control panel layout elements
        self.setup_ui()
        self.draw_cube()

    def setup_ui(self):
        # Top banner instructions
        instructions = tk.Label(
            self.root, 
            text="Click 'Run C++ Solver' to trigger your optimal IDA* engine and watch it solve!",
            fg='white', bg='#222222', font=('Arial', 11, 'bold'), pady=10
        )
        instructions.pack()

        # Visual Grid Canvas Area
        canvas_width = (self.sticker_size * 3 + self.grid_gap) * 4 + 40
        canvas_height = (self.sticker_size * 3 + self.grid_gap) * 3 + 40
        self.canvas = tk.Canvas(self.root, width=canvas_width, height=canvas_height, bg='#333333', highlightthickness=0)
        self.canvas.pack(padx=20, pady=10)

        # Bottom Button Frame Layout
        btn_frame = tk.Frame(self.root, bg='#222222', pady=10)
        btn_frame.pack()

        self.solve_btn = tk.Button(
            btn_frame, text="Run C++ Solver", font=('Arial', 12, 'bold'),
            bg='#00AA00', fg='white', activebackground='#008800', activeforeground='white',
            padx=20, pady=8, command=self.trigger_cpp_solver
        )
        self.solve_btn.pack(side=tk.LEFT, padx=10)

        # Status output tracker line
        self.status_label = tk.Label(self.root, text="Status: Ready", fg='#AAAAAA', bg='#222222', font=('Arial', 10))
        self.status_label.pack(pady=5)

    def draw_cube(self):
        self.canvas.delete("all")
        
        for face, (row_off, col_off) in self.face_offsets.items():
            # Calculate pixel positions for each 3x3 block frame boundary
            start_x = 20 + col_off * (self.sticker_size * 3 + self.grid_gap)
            start_y = 20 + row_off * (self.sticker_size * 3 + self.grid_gap)
            
            # Draw individual square stickers
            for r in range(3):
                for c in range(3):
                    x1 = start_x + c * self.sticker_size
                    y1 = start_y + r * self.sticker_size
                    x2 = x1 + self.sticker_size
                    y2 = y1 + self.sticker_size
                    
                    sticker_color_token = self.state[face][r][c]
                    hex_color = COLORS.get(sticker_color_token, '#000000')
                    
                    self.canvas.create_rectangle(
                        x1, y1, x2, y2,
                        fill=hex_color, outline='#111111', width=2
                    )

    # --- Permutation 2D Tracking Core Rules ---
    def rotate_face_clockwise(self, face):
        self.state[face] = [list(x) for x in zip(*self.state[face][::-1])]

    def apply_move_to_matrix(self, move):
        """Simulates face modifications inside the local 2D layout arrays."""
        if move == "R":
            self.rotate_face_clockwise('R')
            for i in range(3):
                tmp = self.state['U'][i][2]
                self.state['U'][i][2] = self.state['F'][i][2]
                self.state['F'][i][2] = self.state['D'][i][2]
                self.state['D'][i][2] = self.state['B'][2-i][0]
                self.state['B'][2-i][0] = tmp
        elif move == "R'":
            for _ in range(3): self.apply_move_to_matrix("R")
        elif move == "L":
            self.rotate_face_clockwise('L')
            for i in range(3):
                tmp = self.state['U'][i][0]
                self.state['U'][i][0] = self.state['B'][2-i][2]
                self.state['B'][2-i][2] = self.state['D'][i][0]
                self.state['D'][i][0] = self.state['F'][i][0]
                self.state['F'][i][0] = tmp
        elif move == "L'":
            for _ in range(3): self.apply_move_to_matrix("L")
        elif move == "U":
            self.rotate_face_clockwise('U')
            tmp = self.state['F'][0]
            self.state['F'][0] = self.state['R'][0]
            self.state['R'][0] = self.state['B'][0]
            self.state['B'][0] = self.state['L'][0]
            self.state['L'][0] = tmp
        elif move == "U'":
            for _ in range(3): self.apply_move_to_matrix("U")
        elif move == "D":
            self.rotate_face_clockwise('D')
            tmp = self.state['F'][2]
            self.state['F'][2] = self.state['L'][2]
            self.state['L'][2] = self.state['B'][2]
            self.state['B'][2] = self.state['R'][2]
            self.state['R'][2] = tmp
        elif move == "D'":
            for _ in range(3): self.apply_move_to_matrix("D")
        elif move == "F":
            self.rotate_face_clockwise('F')
            for i in range(3):
                tmp = self.state['U'][2][i]
                self.state['U'][2][i] = self.state['L'][2-i][2]
                self.state['L'][2-i][2] = self.state['D'][0][2-i]
                self.state['D'][0][2-i] = self.state['R'][i][0]
                self.state['R'][i][0] = tmp
        elif move == "F'":
            for _ in range(3): self.apply_move_to_matrix("F")
        elif move == "B":
            self.rotate_face_clockwise('B')
            for i in range(3):
                tmp = self.state['U'][0][i]
                self.state['U'][0][i] = self.state['R'][i][2]
                self.state['R'][i][2] = self.state['D'][2][2-i]
                self.state['D'][2][2-i] = self.state['L'][2-i][0]
                self.state['L'][2-i][0] = tmp
        elif move == "B'":
            for _ in range(3): self.apply_move_to_matrix("B")

    def animate_solution_sequence(self, moves):
        """Processes array updates sequentially over timed interval ticks."""
        if not moves:
            self.status_label.config(text="Status: Core Solved Successfully!", fg='#00FF00')
            self.solve_btn.config(state=tk.NORMAL)
            return
            
        current_move = moves.pop(0)
        self.status_label.config(text=f"Animating Engine Move: {current_move}", fg='cyan')
        
        self.apply_move_to_matrix(current_move)
        self.draw_cube()
        
        # Schedule next frame update after 300 milliseconds
        self.root.after(300, lambda: self.animate_solution_sequence(moves))

    def trigger_cpp_solver(self):
        self.solve_btn.config(state=tk.DISABLED)
        self.status_label.config(text="Querying C++ Engine Binary Subprocess...", fg='yellow')
        self.root.update()

        try:
            # 1. Execute your compiled optimal C++ backend binary 
            result = subprocess.run(['./cube_solver.exe'], capture_output=True, text=True, timeout=10)
            lines = result.stdout.split('\n')
            
            # 2. Parse out the optimal path text sequence
            solution_string = ""
            for line in lines:
                if "Solution Path (" in line:
                    print(f"[ENGINE MATCH]: {line}")
                    solution_string = line.split("): ")[1].strip()
                    break
            
            if not solution_string:
                messagebox.showerror("Error", "Could not parse path output string from your engine binary.")
                self.solve_btn.config(state=tk.NORMAL)
                return

            # 3. Reconstruct inverse scramble profile into the UI matrix state 
            # (Matches the hardcoded scramble embedded inside main.cpp)
            scramble = ["R", "U", "F", "B", "L", "L", "D'", "R'", "U", "U", "F'", "L", "B", "B", "D", "R", "U'"]
            for s_move in scramble:
                self.apply_move_to_matrix(s_move)
            self.draw_cube()
            self.root.update()
            
            # Short pause to show the starting scrambled configuration
            time.sleep(1.0)

            # 4. Parse solution steps and expand double turns (e.g. U2 -> U, U)
            raw_moves = solution_string.split(" ")
            expanded_moves = []
            for m in raw_moves:
                if '2' in m:
                    base = m.replace('2', '')
                    expanded_moves.extend([base, base])
                else:
                    expanded_moves.append(m)

            # 5. Kick off timed visual layout updates
            self.animate_solution_sequence(expanded_moves)

        except FileNotFoundError:
            messagebox.showerror("Error", "cube_solver.exe not found! Please build the binary file first.")
            self.solve_btn.config(state=tk.NORMAL)
        except Exception as e:
            messagebox.showerror("Exception", f"Execution Error: {str(e)}")
            self.solve_btn.config(state=tk.NORMAL)

if __name__ == "__main__":
    main_window = tk.Tk()
    app = FlatCubeSolverGUI(main_window)
    main_window.mainloop()