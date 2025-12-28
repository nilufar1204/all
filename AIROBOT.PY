import random 
import datetime
import speech_recognition as sr
import pyttsx3
import time
import warnings
import wikipedia  
import pyaudio 
warnings.filterwarnings('ignore')

class AIAssistant:
    def __init__(self):
        self.name = "R.T."
        self.creator = "Ariq Azmain"
        self.recognizer = sr.Recognizer()
        self.microphone = sr.Microphone()
        self.tts_engine = pyttsx3.init()
        
        # Configure text-to-speech settings
        voices = self.tts_engine.getProperty('voices')
        if voices:
            self.tts_engine.setProperty('voice', voices[0].id)  # Use first available voice
        self.tts_engine.setProperty('rate', 150)  # Speech speed
        
        # Adjust the recognizer sensitivity
        self.recognizer.energy_threshold = 300
        self.recognizer.dynamic_energy_threshold = True
        self.recognizer.pause_threshold = 0.8
        
        print(f"I'm an AI, made by {self.creator} who is a student. My name is {self.name}.")
        self.speak(f"I'm {self.name}, your AI assistant. How can I help you today?")
    
    def speak(self, text):
        """Convert text to speech and print it"""
        print(text)
        try:
            self.tts_engine.say(text)
            self.tts_engine.runAndWait()
        except Exception as e:
            print(f"Text-to-speech error: {e}")
    
    def listen(self):
        """Listen for audio input and convert to text"""
        try:
            with self.microphone as source:
                print("Listening...")
                self.recognizer.adjust_for_ambient_noise(source, duration=0.5)
                audio = self.recognizer.listen(source, timeout=5, phrase_time_limit=5)
            
            print("Processing...")
            query = self.recognizer.recognize_google(audio).lower()
            print(f"User said: {query}")
            return query
        except sr.WaitTimeoutError:
            print("No speech detected within timeout period.")
            return ""
        except sr.UnknownValueError:
            print("Could not understand the audio.")
            return ""
        except sr.RequestError as e:
            print(f"Could not request results from speech recognition service; {e}")
            return ""
        except Exception as e:
            print(f"Unexpected error in speech recognition: {e}")
            return ""
    
    def process_command(self, command):
        """Process the user's command"""
        if not command:
            return False
            
        # Check if the AI was called by name
        if self.name.lower() in command or 'rt' in command:
            command = command.replace(self.name.lower(), '').replace('rt', '').strip()
        
        li2 = [
            'Ask me anything.',
            'Do you have any questions?',
            'How can I help you?',
            'Ask me more questions',
            "I'm ready to answer your questions, ask me."
        ]
        l2 = random.choice(li2)
        
        try:
            if 'time' in command:
                current_time = datetime.datetime.now().strftime('%I:%M %p')
                li1 = [
                    'Current time is ',
                    'The time is ',
                    'Now it is '
                ]
                l1 = random.choice(li1)
                response = f"{l1}{current_time}"
                self.speak(response)
                self.speak(l2)
                return True
            
            elif 'tell me about' in command:
                topic = command.replace('tell me about', '').strip()
                if topic:
                    self.speak(f"Searching for information about {topic}")
                    info = wikipedia.summary(topic, sentences=2)
                    self.speak(info)
                    self.speak("Wikipedia feature is currently disabled. Please enable it by uncommenting the wikipedia import.")
                else:
                    self.speak("What would you like me to tell you about?")
                return True
                
            elif 'what is' in command:
                topic = command.replace('what is', '').strip()
                if topic:
                    self.speak(f"Searching for information about {topic}")
                    info = wikipedia.summary(topic, sentences=2)
                    self.speak(info)
                    self.speak("Wikipedia feature is currently disabled. Please enable it by uncommenting the wikipedia import.")
                else:
                    self.speak("What would you like me to explain?")
                return True
                
            elif 'stop' in command or 'exit' in command or 'quit' in command:
                self.speak("Goodbye! Have a great day.")
                return False
                
            else:
                self.speak("I'm not sure how to help with that. Please try asking something else.")
                self.speak(l2)
                return True
                
        except Exception as e:
            error_message = f"I encountered an error: {str(e)}. Please try again."
            self.speak(error_message)
            return True

def main():
    assistant = AIAssistant()
    use_voice = input("Would you like to use voice input? (yes/no): ").lower().strip()
    
    if use_voice.startswith('y'):
        assistant.speak("Voice mode activated. Say my name RT followed by your question.")
        
        while True:
            # Listen for wake word
            try:
                with assistant.microphone as source:
                    assistant.recognizer.adjust_for_ambient_noise(source, duration=0.5)
                    audio = assistant.recognizer.listen(source, timeout=3, phrase_time_limit=3)
                
                wake_word = assistant.recognizer.recognize_google(audio).lower()
                print(f"Heard: {wake_word}")
                
                if assistant.name.lower() in wake_word or 'rt' in wake_word:
                    assistant.speak("Yes? How can I help you?")
                    command = assistant.listen()
                    if not assistant.process_command(command):
                        break
                        
            except sr.WaitTimeoutError:
                # No wake word detected, continue listening
                pass
            except sr.UnknownValueError:
                # Could not understand audio, continue listening
                pass
            except Exception as e:
                print(f"Error in wake word detection: {e}")
    else:
        # Text mode
        assistant.speak("Text mode activated. You can type your questions.")
        while True:
            command = input('Ask me: ').lower()
            if not assistant.process_command(command):
                break

if __name__ == "__main__":
    main()