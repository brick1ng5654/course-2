bool is_in_list(int element, Node *list){
    while(list){
        if(element == list->data) return true;
        else list = list->next;
    }
    return false;
}

void association(Node *&res_list, Node *ad_list){
    while(ad_list){
        if(!is_in_list(ad_list->data, res_list)){push_back(res_list, ad_list->data);}
        ad_list = ad_list->next;
    }
}

void intersection(Node *&res_list,Node *ad1_list, Node *ad2_list){
    while(ad1_list){
        if(is_in_list(ad1_list->data, ad2_list)){
            push_back(res_list, ad1_list->data);
        }
        ad1_list = ad1_list->next;
    }
}