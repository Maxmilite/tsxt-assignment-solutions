def select_word(text):
    result = 0
    for i in text:
        if (len(i) == 1):
            result += 1
    return result


def fully_segment(text, dic):
    word_list = []
    for i in range(len(text)):
        for j in range(i+1, len(text)+1):
            word = text[i:j]
            if word in dic:
                word_list.append(word)
    return word_list


def forward_segment(text, dic):
    word_list = []
    i = 0
    while (i < len(text)):
        longest_word = text[i]
        for j in range(i+1, len(text)+1):
            word = text[i:j]
            if word in dic:
                if len(word) > len(longest_word):
                    longest_word = word
        word_list.append(longest_word)
        i += len(longest_word)
    return word_list


def backward_segment(text, dic):
    word_list = []
    i = len(text)-1
    while (i >= 0):
        longest_word = text[i]
        for j in range(0, i):
            word = text[j:i+1]
            if word in dic:
                if len(word) > len(longest_word):
                    longest_word = word
        word_list.insert(0, longest_word)
        i -= len(longest_word)
    return word_list


def all_segment(text, dic):
    list_forward = forward_segment(text, dic)
    list_backward = backward_segment(text, dic)
    list_final = []
    if (len(list_forward) > len(list_backward)):
        list_final = list_backward[:]
    elif (len(list_forward) < len(list_backward)):
        list_final = list_forward[:]
    else:
        if (select_word(list_forward) > select_word(list_backward)):
            list_final = list_backward[:]
        elif (select_word(list_forward) < select_word(list_backward)):
            list_final = list_forward[:]
        else:
            list_final = list_backward[:]
    return list_final


dic = [
    'nice', 'meet', '中文', 'ChatGPT', 'how', 'going', 'give', 'until', 'believe', 'think', 'action', 'never', 'winners', 'jack'
]

def getNLP(e):
    return fully_segment(e, dic)
