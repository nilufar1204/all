from kivy.app import App
from kivy.uix.widget import Widget
from kivy.graphics import Color, Rectangle
from kivy.core.window import Window
from kivy.clock import Clock

class SimpleGame(Widget):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.player = Rectangle(pos=(Window.width/2, 10), size=(50, 50))
        with self.canvas:
            Color(1, 0, 0)
            self.player = Rectangle(pos=(Window.width/2, 10), size=(50, 50))

    def update(self, dt):
        self.player.pos = (self.player.pos[0] + 1, self.player.pos[1])

class TestApp(App):
    def build(self):
        game = SimpleGame()
        Clock.schedule_interval(game.update, 1/60)
        return game

TestApp().run()