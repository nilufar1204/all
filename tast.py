import random
from typing import *

class Agent:
    def __init__(self, name: str, knowledge_base: Dict = None):
        self.name = name
        self.knowledge_base = knowledge_base or {
            "general": {
                "name": "সহায়ক",
                "creator": "আপনি",
                "status": "আমি ভাল আছি, ধন্যবাদ!"
            },
            "facts": {
                "বাংলাদেশের রাজধানী": "ঢাকা",
                "ভারতের রাজধানী": "নয়াদিল্লী",
                "বাংলাদেশের মুদ্রা": "টাকা",
                "ভারতের মুদ্রা": "রুপি",
                "এক ও এক": "২ (দুই)"
            },
            "math": {
                "এক ও এক": "২ (দুই)",
                "দুই ও দুই": "৪ (চার)",
                
            }
        }
        self.memory = [knowledge_base]
    
    def perceive(self, observation: str):
        self.memory.append(observation)
        print(f"{self.name} পর্যবেক্ষণ করল: {observation}")
    
    def think(self):
        if not self.memory:
            return None
        
        latest_observation = self.memory[-1].lower()
        
        # নাম সম্পর্কিত প্রশ্ন
        if any(q in latest_observation for q in ["নাম কি", "কে তুমি", "তোমার নাম"]):
            return "state_name"
        # সাধারণ তথ্য জানতে চাওয়া
        elif any(keyword in latest_observation for keyword in ["রাজধানী", "মুদ্রা", "জনসংখ্যা"]):
            return "answer_fact"
        # গাণিতিক প্রশ্ন
        elif any(op in latest_observation for op in ["+", "ও", "যোগ", "কতো"]):
            return "answer_math"
        # সম্ভাষণ
        elif any(greeting in latest_observation for greeting in ["হ্যালো", "হাই", "কেমন আছ"]):
            return "greet"
        elif "ধন্যবাদ" in latest_observation:
            return "acknowledge_thanks"
        else:
            return "continue_conversation"
    
    def act(self, action: str) -> str:
        action_handlers = {
            "greet": self._handle_greet,
            "answer_fact": self._handle_answer_fact,
            "answer_math": self._handle_answer_math,
            "acknowledge_thanks": self._handle_acknowledge_thanks,
            "continue_conversation": self._handle_continue_conversation,
            "state_name": self._handle_state_name
        }
        
        handler = action_handlers.get(action, self._handle_unknown)
        return handler()
    
    def _handle_greet(self) -> str:
        responses = [
            f"হ্যালো! আমি {self.knowledge_base['general']['name']}, কিভাবে আপনাকে সাহায্য করতে পারি?",
            f"{self.knowledge_base['general']['status']} আপনি কিভাবে আছেন?",
            "শুভেচ্ছা! আমি আপনার সহায়তার জন্য এখানে আছি।"
        ]
        return random.choice(responses)
    
    def _handle_answer_fact(self) -> str:
        latest_question = self.memory[-1]
        
        for fact in self.knowledge_base["facts"]:
            if fact.lower() in latest_question.lower():
                return f"{fact} হল {self.knowledge_base['facts'][fact]}"
        
        return "আমি সেই তথ্যটি জানি না। আপনি অন্য কিছু জানতে চান?"
    
    def _handle_answer_math(self) -> str:
        latest_question = self.memory[-1].lower()
        
        for problem in self.knowledge_base["math"]:
            if problem.lower() in latest_question:
                return f"{problem} এর উত্তর হল {self.knowledge_base['math'][problem]}"
        
        return "আমি সেই গাণিতিক সমস্যাটির সমাধান জানি না।"
    
    def _handle_acknowledge_thanks(self) -> str:
        return "আপনাকে ধন্যবাদ! আর কিছু জানার থাকলে আমাকে বলুন।"
    
    def _handle_continue_conversation(self) -> str:
        prompts = [
            "আপনি আরও কিছু বলতে চান?",
            "আমি আপনাকে কিভাবে আরও সাহায্য করতে পারি?",
            "আপনার কি অন্য কোন প্রশ্ন আছে?"
        ]
        return random.choice(prompts)
    
    def _handle_state_name(self) -> str:
        return f"আমার নাম {self.knowledge_base['general']['name']}"
    
    def _handle_unknown(self) -> str:
        return "আমি নিশ্চিত না যে আপনি কি বলতে চাচ্ছেন। দয়া করে পরিষ্কার করে বলুন।"

class Environment:
    def __init__(self):
        self.agents = {}
    
    def add_agent(self, agent: Agent):
        self.agents[agent.name] = agent
    
    def interact(self, agent_name: str, input_text: str):
        if agent_name not in self.agents:
            return "এজেন্ট খুঁজে পাওয়া যায়নি"
        
        agent = self.agents[agent_name]
        agent.perceive(input_text)
        action = agent.think()
        response = agent.act(action)
        return response

if __name__ == "__main__":
    env = Environment()
    assistant = Agent("সহায়ক")
    env.add_agent(assistant)
    
    print("এজেন্টিক AI সিস্টেমে স্বাগতম (প্রস্থানের জন্য 'exit' লিখুন)")
    while True:
        user_input = input("আপনি: ")
        if user_input.lower() == 'exit':
            break
        
        response = env.interact("সহায়ক", user_input)
        print(f"সহায়ক: {response}")
