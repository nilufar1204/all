#ai
import spacy
import speech_recognition as sr
import pyttsx3
import datetime
import pywhatkit
import wikipedia

# SpaCy মডেল লোড করা
nlp = spacy.load("bn_core_news_sm")

# TTS ইঞ্জিন
engine = pyttsx3.init()
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[0].id)

def speak(audio):
    engine.say(audio)
    engine.runAndWait()

def take_command():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("শুনছি...")
        r.pause_threshold = 1
        audio = r.listen(source)
    try:
        query = r.recognize_google(audio, language='bn-BD')
        print(f"আপনি বলেছেন: {query}\n")
    except:
        return "None"
    return query

def get_intent_spacy(command):
    doc = nlp(command)

    if any(token.text in ['সময়', 'কটা', 'বাজে'] for token in doc):
        if not any(ent.label_ == 'WORK_OF_ART' for ent in doc.ents):
            return 'get_time'
    
    if any(token.text in ['গান', 'চালাও'] for token in doc):
        return 'play_song'

    # SpaCy Named Entity Recognition (NER) ব্যবহার করে বইয়ের নাম খোঁজা
    for ent in doc.ents:
        if ent.label_ == 'WORK_OF_ART': # 'WORK_OF_ART' সাধারণত বই, গান বা চলচ্চিত্রের নাম নির্দেশ করে
            return 'get_book_info'

    if any(token.text in ['উইকিপিডিয়া', 'সম্পর্কে'] for token in doc):
        return 'get_wiki_info'

    if any(token.text in ['গুগল', 'খোঁজো', 'সার্চ'] for token in doc):
        return 'Google Search'

    return 'unknown'

def run_ai():
    speak("নমস্কার, আমি আপনার জন্য কী করতে পারি?")
    while True:
        command = take_command().lower()
        intent = get_intent_spacy(command)
        
        if intent == 'get_time':
            time = datetime.datetime.now().strftime('%I:%M %p')
            speak(f"এখন সময় হলো {time}")
        
        elif intent == 'play_song':
            song = command.replace('গান চালাও', '').strip()
            speak(f"{song} গানটি চালাচ্ছি")
            pywhatkit.playonyt(song)
        
        elif intent == 'get_wiki_info':
            speak('উইকিপিডিয়ায় কি খুঁজছেন?')
            search_query = take_command().lower()
            try:
                result = wikipedia.summary(search_query, sentences=2)
                speak("উইকিপিডিয়া থেকে প্রাপ্ত ফলাফল:")
                speak(result)
            except:
                speak("দুঃখিত, এই বিষয়ে কোনো তথ্য পাওয়া যায়নি।")
        
        elif intent == 'Google Search':
            speak('গুগলে কি খুঁজছেন?')
            search_query = take_command().lower()
            pywhatkit.search(search_query)

        elif intent == 'get_book_info':
            doc = nlp(command)
            book_name = ""
            for ent in doc.ents:
                if ent.label_ == 'WORK_OF_ART':
                    book_name = ent.text
            
            if book_name:
                speak(f"{book_name} বইটি সম্পর্কে উইকিপিডিয়ায় খুঁজছি।")
                try:
                    result = wikipedia.summary(book_name, sentences=2)
                    speak(result)
                except:
                    speak("দুঃখিত, এই বইটি সম্পর্কে কোনো তথ্য পাওয়া যায়নি।")
            else:
                speak("কোন বই সম্পর্কে জানতে চান?")

        elif 'বিদায়' in command or 'বন্ধ করো' in command:
            speak("বিদায়, ভালো থাকবেন।")
            break

        else:
            if intent == 'unknown':
                speak("দুঃখিত, আমি বুঝতে পারিনি।")

if __name__ == "__main__":
    run_ai()