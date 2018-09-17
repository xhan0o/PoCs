import praw
import json
import datetime


AUTH_FILE_NAME = 'auth.json'

class PrawInitData:
	def __init__(self, botDetails):
		self.client_id = botDetails['client_id']
		self.client_secret = botDetails['client_secret']
		self.user_agent = botDetails['user_agent']
		self.username = botDetails['username']
		self.password = botDetails['password']


def make_praw_instance(init_data):
	return praw.Reddit(client_id=init_data.client_id, client_secret=init_data.client_secret, password=init_data.password,
					   user_agent=init_data.user_agent, username=init_data.username)

def get_titles_from_praw(reddit, n):
	return [submission.title for submission in reddit.subreddit('worldnews').hot(limit=n)]

def initialize_praw():
    with open(AUTH_FILE_NAME) as f:
        data = json.load(f)
    bot1 = data['reddit'][0]
    return PrawInitData(bot1)

def whoami():
    praw_init_fields = initialize_praw()
    reddit = make_praw_instance(praw_init_fields)
    return (reddit.user.me())

def streamSub(sub):
    praw_init_fields = initialize_praw()
    reddit = make_praw_instance(praw_init_fields)
    subreddit = reddit.subreddit(sub)
    for submission in subreddit.stream.submissions():
        try:
            print datetime.datetime.fromtimestamp(submission.created_utc)
            print (submission.title).encode('utf-8') + "\t" + str(submission.url) + "\t u/"+ str(submission.author) + " r/" + str(submission.subreddit)
            print "Perform the task"
        except Exception as e:
            pass
    return 0
def top_posts(sublist,n):
    top25 = [{}]
    for subs in sublist:
        subreddit = reddit.subreddit(subs).new(limit=n)
        for submission in subreddit:
            top25.append({str((submission.title).encode('utf-8')) + " user: " + str(submission.author) : submission.url})
    return top25
#print whoami()
#top_posts (['programminghumor'],20)
#streamSub('programmerhumor')
#imageList(reddit)
