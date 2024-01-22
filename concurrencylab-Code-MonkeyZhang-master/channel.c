#include "channel.h"

// Creates a new channel with the provided size and returns it to the caller
// A 0 size indicates an unbuffered channel, whereas a positive size indicates a buffered channel
channel_t *channel_create(size_t size)
{
    /* IMPLEMENT THIS */
    // malloc the channel
    channel_t *channel = malloc(sizeof(channel_t));

    // create a buffer of size size
    buffer_t *buffer = buffer_create(size);
    channel->buffer = buffer;

    // initialize the mutex and condition variables
    pthread_mutex_init(&channel->mutex, NULL);
    pthread_cond_init(&channel->send_cond, NULL);
    pthread_cond_init(&channel->recv_cond, NULL);

    // initialize other variables
    channel->send_wait_count = 0;
    channel->recv_wait_count = 0;
    channel->is_closed = false;

    // initialize select semaphore
    channel->select_send_sem = NULL;
    channel->select_recv_sem = NULL;

    // create a list to store the select send and receive semaphores
    channel->send_sem_list = list_create();
    channel->recv_sem_list = list_create();

    return channel;
}

// Writes data to the given channel
// This is a blocking call i.e., the function only returns on a successful completion of send
// In case the channel is full, the function waits till the channel has space to write the new data
// Returns SUCCESS for successfully writing data to the channel,
// CLOSED_ERROR if the channel is closed, and
// GEN_ERROR on encountering any other generic error of any sort
enum channel_status channel_send(channel_t *channel, void *data)
{

    // lock the mutex
    pthread_mutex_lock(&channel->mutex);

    // check if the channel is closed
    if (channel->is_closed)
    {
        pthread_mutex_unlock(&channel->mutex);
        return CLOSED_ERROR;
    }

    // loop checking if the buffer is full
    // if the buffer is full, wait on the send condition variable
    while (channel->buffer->size == channel->buffer->capacity)
    {
        // increment the send_wait_count
        channel->send_wait_count++;
        pthread_cond_wait(&channel->send_cond, &channel->mutex);

        // when the thread wakes up, check if the channel is closed
        // because channel_close() will boardcast all the send condition variable
        if (channel->is_closed)
        {
            channel->send_wait_count--;
            pthread_mutex_unlock(&channel->mutex);
            return CLOSED_ERROR;
        }
        channel->send_wait_count--;
    }

    // otherwise add the data to the buffer
    buffer_add(channel->buffer, data);
    // signal the receive condition variable
    pthread_cond_signal(&channel->recv_cond);

    // notify all the recv semaphores list
    list_node_t *current = channel->recv_sem_list->head;
    for (size_t i = 0; i < channel->recv_sem_list->count; i++)
    {
        sem_post(current->data);
        current = current->next;
    }

    // unlock the mutex
    pthread_mutex_unlock(&channel->mutex);
    return SUCCESS;
}

// Reads data from the given channel and stores it in the function's input parameter, data (Note that it is a double pointer)
// This is a blocking call i.e., the function only returns on a successful completion of receive
// In case the channel is empty, the function waits till the channel has some data to read
// Returns SUCCESS for successful retrieval of data,
// CLOSED_ERROR if the channel is closed, and
// GEN_ERROR on encountering any other generic error of any sort
enum channel_status channel_receive(channel_t *channel, void **data)
{

    // lock the mutex
    pthread_mutex_lock(&channel->mutex);

    // check if the channel is closed
    if (channel->is_closed)
    {
        pthread_mutex_unlock(&channel->mutex);
        return CLOSED_ERROR;
    }

    // loop checking if the buffer is empty
    // if the buffer is empty, wait on the receive condition variable
    while (channel->buffer->size == 0)
    {
        channel->recv_wait_count++;
        pthread_cond_wait(&channel->recv_cond, &channel->mutex);
        // when the thread wakes up, check if the channel is closed
        if (channel->is_closed)
        {
            channel->recv_wait_count--;
            pthread_mutex_unlock(&channel->mutex);
            return CLOSED_ERROR;
        }
        channel->recv_wait_count--;
    }

    // if the buffer is not empty, remove the data from the buffer
    buffer_remove(channel->buffer, data);
    // signal the send condition variable
    pthread_cond_signal(&channel->send_cond);

    // notify all the send semaphores list
    list_node_t *current = channel->send_sem_list->head;
    for (size_t i = 0; i < channel->send_sem_list->count; i++)
    {
        sem_post(current->data);
        current = current->next;
    }

    // unlock the mutex
    pthread_mutex_unlock(&channel->mutex);
    return SUCCESS;
}

// Writes data to the given channel
// This is a non-blocking call i.e., the function simply returns if the channel is full
// Returns SUCCESS for successfully writing data to the channel,
// CHANNEL_FULL if the channel is full and the data was not added to the buffer,
// CLOSED_ERROR if the channel is closed, and
// GEN_ERROR on encountering any other generic error of any sort
enum channel_status channel_non_blocking_send(channel_t *channel, void *data)
{
    // lock the mutex
    pthread_mutex_lock(&channel->mutex);

    // check if the channel is closed
    if (channel->is_closed)
    {
        pthread_mutex_unlock(&channel->mutex);
        return CLOSED_ERROR;
    }

    //  if the buffer is full
    //  unlock the mutex and return CHANNEL_FULL
    if (channel->buffer->size == channel->buffer->capacity)
    {
        pthread_mutex_unlock(&channel->mutex);
        return CHANNEL_FULL;
    }

    // otherwise add the data to the buffer
    buffer_add(channel->buffer, data);
    // // signal the receive condition variable
    if (channel->recv_wait_count > 0)
    {
        pthread_cond_signal(&channel->recv_cond);
    }

    // // notify the select recv semaphore
    // if (channel->select_recv_sem != NULL)
    // {
    //     sem_post(channel->select_recv_sem);
    // }

    // notify all the recv semaphores list
    list_node_t *current = channel->recv_sem_list->head;
    for (size_t i = 0; i < channel->recv_sem_list->count; i++)
    {
        sem_post(current->data);
        current = current->next;
    }

    // unlock the mutex
    pthread_mutex_unlock(&channel->mutex);
    return SUCCESS;
}

// Reads data from the given channel and stores it in the function's input parameter data (Note that it is a double pointer)
// This is a non-blocking call i.e., the function simply returns if the channel is empty
// Returns SUCCESS for successful retrieval of data,
// CHANNEL_EMPTY if the channel is empty and nothing was stored in data,
// CLOSED_ERROR if the channel is closed, and
// GEN_ERROR on encountering any other generic error of any sort
enum channel_status channel_non_blocking_receive(channel_t *channel, void **data)
{
    /* IMPLEMENT THIS */
    // lock the mutex
    pthread_mutex_lock(&channel->mutex);

    // check if the channel is closed
    if (channel->is_closed)
    {
        pthread_mutex_unlock(&channel->mutex);
        return CLOSED_ERROR;
    }

    // if the buffer is empty
    // unlock the mutex and return CHANNEL_EMPTY
    if (channel->buffer->size == 0)
    {
        pthread_mutex_unlock(&channel->mutex);
        return CHANNEL_EMPTY;
    }

    // otherwise remove the data from the buffer
    buffer_remove(channel->buffer, data);
    // signal the send condition variable
    if (channel->send_wait_count > 0)
    {
        pthread_cond_signal(&channel->send_cond);
    }

    // // notify the select send semaphore
    // if (channel->select_send_sem != NULL)
    // {
    //     sem_post(channel->select_send_sem);
    // }

    // notify all the send semaphores list
    list_node_t *current = channel->send_sem_list->head;
    for (size_t i = 0; i < channel->send_sem_list->count; i++)
    {
        sem_post(current->data);
        current = current->next;
    }

    // unlock the mutex
    pthread_mutex_unlock(&channel->mutex);
    return SUCCESS;
}

// Closes the channel and informs all the blocking send/receive/select calls to return with CLOSED_ERROR
// Once the channel is closed, send/receive/select operations will cease to function and just return CLOSED_ERROR
// Returns SUCCESS if close is successful,
// CLOSED_ERROR if the channel is already closed, and
// GEN_ERROR in any other error case
enum channel_status channel_close(channel_t *channel)
{
    // lock the mutex
    pthread_mutex_lock(&channel->mutex);
    // check if the channel is already closed
    if (channel->is_closed)
    {
        pthread_mutex_unlock(&channel->mutex);
        return CLOSED_ERROR;
    }
    else
    {
        // set the is_closed flag to true
        channel->is_closed = true;

        // broadcast the condition variables
        pthread_cond_broadcast(&channel->send_cond);
        pthread_cond_broadcast(&channel->recv_cond);

        // notify all semaphores in the list
        // notify the  send semaphore list
        list_node_t *node = channel->send_sem_list->head;
        for (int i = 0; i < channel->send_sem_list->count; i++)
        {
            sem_post(node->data);
            node = node->next;
        }
        // notify the  recv semaphore list
        node = channel->recv_sem_list->head;
        for (int i = 0; i < channel->recv_sem_list->count; i++)
        {
            sem_post(node->data);
            node = node->next;
        }

        // unlock the mutex
        pthread_mutex_unlock(&channel->mutex);
        return SUCCESS;
    }
    pthread_mutex_unlock(&channel->mutex);
    return GEN_ERROR;
}

// Frees all the memory allocated to the channel
// The caller is responsible for calling channel_close and waiting for all threads to finish their tasks before calling channel_destroy
// Returns SUCCESS if destroy is successful,
// DESTROY_ERROR if channel_destroy is called on an open channel, and
// GEN_ERROR in any other error case
enum channel_status channel_destroy(channel_t *channel)
{
    /* IMPLEMENT THIS */

    // DESTROY_ERROR if channel_destroy is called on an open channel
    if (channel->is_closed == false)
    {
        return DESTROY_ERROR;
    }
    else
    {
        // Undo everything in channel_create()
        // destroy mutex and condition variables
        pthread_mutex_destroy(&channel->mutex);
        pthread_cond_destroy(&channel->send_cond);
        pthread_cond_destroy(&channel->recv_cond);

        // free semophores
        if (channel->select_send_sem != NULL)
        {
            free(channel->select_send_sem);
        }
        if (channel->select_recv_sem != NULL)
        {
            free(channel->select_recv_sem);
        }

        // free lists
        if (channel->send_sem_list != NULL)
        {
            list_destroy(channel->send_sem_list);
        }
        if (channel->recv_sem_list != NULL)
        {
            list_destroy(channel->recv_sem_list);
        }

        // free the allocated memory
        buffer_free(channel->buffer);
        free(channel);
        return SUCCESS;
    }
    return GEN_ERROR;
}

// Takes an array of channels (channel_list) of type select_t and the array length (channel_count) as inputs
// This API iterates over the provided list and finds the set of possible channels which can be used to invoke the required operation (send or receive) specified in select_t
// If multiple options are available, it selects the first option and performs its corresponding action
// If no channel is available, the call is blocked and waits till it finds a channel which supports its required operation
// Once an operation has been successfully performed, select should set selected_index to the index of the channel that performed the operation and then return SUCCESS
// In the event that a channel is closed or encounters any error, the error should be propagated and returned through select
// Additionally, selected_index is set to the index of the channel that generated the error
enum channel_status channel_select(select_t *channel_list, size_t channel_count, size_t *selected_index)
{
    /* IMPLEMENT THIS */

    // create semaphore for each channel
    sem_t sem;
    sem_init(&sem, 0, 0);

    // add semaphore to the each channel's list
    for (size_t i = 0; i < channel_count; i++)
    {
        // Lock
        pthread_mutex_lock(&channel_list[i].channel->mutex);

        // if this channel is closed, return CLOSED_ERROR
        if (channel_list[i].channel->is_closed == true)
        {
            pthread_mutex_unlock(&channel_list[i].channel->mutex);
            return CLOSED_ERROR;
        }

        // add sem to list
        // if the channel is SEND, add sem to send_sem_list
        if (channel_list[i].dir == SEND)
        {
            // condition check? if sem is already in the list, don't add it again
            list_insert(channel_list[i].channel->send_sem_list, &sem);
        }

        // if the channel is RECV, add sem to recv_sem_list
        else if (channel_list[i].dir == RECV)
        {
            // if (list_find(channel_list[i].channel->recv_sem_list, &sem) == NULL)
            // {
            //     list_insert(channel_list[i].channel->recv_sem_list, &sem);
            // }
            list_insert(channel_list[i].channel->recv_sem_list, &sem);
        }

        // Unlock
        pthread_mutex_unlock(&channel_list[i].channel->mutex);
    }

    while (true)
    {
        // store channel status
        enum channel_status status;
        // iterate through the channel list
        for (size_t i = 0; i < channel_count; i++)
        {
            // Sender channel
            if (channel_list[i].dir == SEND)
            {
                // perform send operation
                status = channel_non_blocking_send(channel_list[i].channel, channel_list[i].data);
                // if channel is full, go to next channel
                if (status == CHANNEL_FULL)
                {
                    continue;
                }
                // if channel is closed or Success, return
                else if (status == CLOSED_ERROR || status == SUCCESS || status == GEN_ERROR || status == DESTROY_ERROR)
                {
                    pthread_mutex_lock(&channel_list[i].channel->mutex);
                    // remove semaphore from channel's list
                    for (size_t i = 0; i < channel_count; i++)
                    {

                        // remove sem from send_sem_list
                        // find the node that contains sem
                        list_node_t *remove_node = list_find(channel_list[i].channel->send_sem_list, &sem);
                        if (remove_node != NULL)
                        {
                            list_remove(channel_list[i].channel->send_sem_list, remove_node);
                        }
                    }
                    // unlock the mutex
                    pthread_mutex_unlock(&channel_list[i].channel->mutex);

                    *selected_index = i;
                    sem_destroy(&sem);
                    return status;
                }
            }

            // Receiver channel
            else if (channel_list[i].dir == RECV)
            {
                // perform send operation
                status = channel_non_blocking_receive(channel_list[i].channel, &channel_list[i].data);
                // if channel is empty, go to next channel
                if (status == CHANNEL_EMPTY)
                {
                    continue;
                }
                // if channel is closed or Success, return
                else if (status == CLOSED_ERROR || status == SUCCESS || status == GEN_ERROR || status == DESTROY_ERROR)
                {
                    pthread_mutex_lock(&channel_list[i].channel->mutex);
                    // remove semaphore from channel
                    list_node_t *remove_node = list_find(channel_list[i].channel->recv_sem_list, &sem);
                    for (size_t i = 0; i < channel_count; i++)
                    {
                        if (remove_node != NULL)
                        {
                            list_remove(channel_list[i].channel->recv_sem_list, remove_node);
                        }
                    }
                    // unlock the mutex
                    pthread_mutex_unlock(&channel_list[i].channel->mutex);

                    *selected_index = i;
                    sem_destroy(&sem);
                    return status;
                }
            }
        }
        // if all channels are unavailable, wait for the semaphore
        sem_wait(&sem);
    }
    return GEN_ERROR;
}
