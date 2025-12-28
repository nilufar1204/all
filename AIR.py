import random 
import datetime 
import speech_recognition as sr
import pyttsx3
import wikipedia
import pywhatkit
import time

# Initialize text-to-speech engine
engine = pyttsx3.init()

# Function to speak text
def speak(text):
    print(text)
    try:
        engine.say(text)
        engine.runAndWait()
    except Exception as e:
        print(f"Text-to-speech error: {e}")

# Function to listen for voice commands
def listen():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Listening...")
        r.adjust_for_ambient_noise(source, duration=1)
        audio = r.listen(source, timeout=5, phrase_time_limit=5)
    
    try:
        query = r.recognize_google(audio)
        print(f"You said: {query}")
        return query.lower()
    except sr.UnknownValueError:
        speak("Sorry, I didn't understand that.")
        return ""
    except sr.RequestError as e:
        speak(f"Could not request results from speech recognition service; {e}")
        return ""
    except Exception as e:
        speak(f"Unexpected error: {e}")
        return ""

# Main program
speak("I'm an AI, made by Ariq Azmain who is a student. My name is R.T.")

# Ask if user wants voice mode
speak("Would you like to enable voice mode? Please say yes or no.")
response = input("Enable voice mode? (yes/no): ").lower()

if response in ['yes', 'y']:
    voice_mode = True
    speak("Voice mode activated. You can say 'RT' to get my attention.")
else:
    voice_mode = False
    speak("Text mode activated. You can type your questions.")

while True:
    if voice_mode:
        # Listen for wake word
        try:
            r = sr.Recognizer()
            with sr.Microphone() as source:
                print("Listening for wake word...")
                r.adjust_for_ambient_noise(source, duration=1)
                audio = r.listen(source, timeout=3, phrase_time_limit=3)
            
            wake_word = r.recognize_google(audio).lower()
            print(f"Heard: {wake_word}")
            
            if 'rt' in wake_word or 'r.t' in wake_word or 'artie' in wake_word:
                speak("Yes, how can I help you?")
                command = listen()
            else:
                continue
        except:
            continue
    else:
        command = input('Ask me: ').lower()
    
    li2 = ['Ask me anything.', 'Do you have any questions?', 'How can I help you?', 'Ask me more questions', "I'm ready to answer your questions, ask me."]
    l2 = random.choice(li2)
    
    try:
        if 'time' in command:
            current_time = datetime.datetime.now().strftime('%I:%M %p')
            li1 = ['Current time is ', 'The time is ', 'Now it is ']
            l1 = random.choice(li1)
            response = f"{l1}{current_time}"
            speak(response)
            speak(l2)
        
        elif 'tell me about' in command:
            topic = command.replace('tell me about', '').strip()
            if topic:
                info = wikipedia.summary(topic, sentences=2)
                speak(info)
            else:
                speak("What would you like me to tell you about?")
        
        elif 'what is' in command:
            topic = command.replace('what is', '').strip()
            if topic:
                info = wikipedia.summary(topic, sentences=2)
                speak(info)
            else:
                speak("What would you like me to explain?")
        
        elif 'play' in command:
            song = command.replace('play', '').strip()
            if song:
                speak(f"Playing {song} on YouTube")
                pywhatkit.playonyt(song)
            else:
                speak("What would you like me to play?")
        
        elif 'search' in command:
            query = command.replace('search', '').strip()
            if query:
                speak(f"Searching for {query}")
                pywhatkit.search(query)
            else:
                speak("What would you like me to search for?")
        
        elif 'stop' in command or 'exit' in command or 'quit' in command:
            speak("Goodbye. Have a nice day.")
            break
        
        else:
            speak("I don't know that. Please tell me again.")
            
    except wikipedia.exceptions.DisambiguationError as e:
        speak(f"There are multiple results for that. Please be more specific. Options include: {', '.join(e.options[:3])}")
    
    except wikipedia.exceptions.PageError:
        speak("Sorry, I couldn't find any information on that topic.")
    
    except Exception as e:
        error_message = f"Sorry, an error occurred: {str(e)}. Please try again."
        speak(error_message)