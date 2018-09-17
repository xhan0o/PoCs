from telegram.ext import Updater
from telegram.ext import CommandHandler
import json
import reddit_wrap as red
import datetime

AUTH_FILE_NAME = 'auth.json'

def initialize_telegram():
    with open(AUTH_FILE_NAME) as f:
        data = json.load(f)
    token = data['telegram'][0]['token']
    return (token)
token = initialize_telegram()
updater = Updater(token=token)
dispatcher = updater.dispatcher


def start(bot, update):
    bot.send_message(chat_id=update.message.chat_id, text="I'm a bot, please talk to me!")

def nerdMemes(bot, update):
    praw_init_fields = red.initialize_praw()
    reddit = red.make_praw_instance(praw_init_fields)
    sublist = ['programmerhumor']
    for subs in sublist:
        subreddit = reddit.subreddit(subs).new(limit=10)
        for submission in subreddit:
            print submission.title
            try:
                bot.sendPhoto(chat_id = update.message.chat_id, photo = str(submission.url), caption = submission.title.encode('utf-8'))
            except:
                pass

def streamer(bot,update):
    praw_init_fields = red.initialize_praw()
    reddit = red.make_praw_instance(praw_init_fields)
    subreddit = reddit.subreddit('programmerhumor+funnygif')
    for submission in subreddit.stream.submissions():
        try:
            print datetime.datetime.fromtimestamp(submission.created_utc)
            print (submission.title).encode('utf-8') + "\t" + str(submission.url) + "\t u/"+ str(submission.author) + " r/" + str(submission.subreddit)
            bot.sendPhoto(chat_id = update.message.chat_id, photo = str(submission.url), caption = submission.title.encode('utf-8'))
        except Exception as e:
            print e
            pass

start_handler = CommandHandler('start', start)
nerdMemes_handler = CommandHandler('memes', nerdMemes)
streamer_handler = CommandHandler('streamer', streamer)
dispatcher.add_handler(start_handler)
dispatcher.add_handler(nerdMemes_handler)
dispatcher.add_handler(streamer_handler)
#dispatcher.add_error_handler(error_callback)
updater.start_polling()
