import pika
import uuid

connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()

result = channel.queue_declare('', exclusive=True)
callback_queue = result.method.queue

corr_id = str(uuid.uuid4())

def on_response(ch, method, props, body):
    if props.correlation_id == corr_id:
        print("Response:", body.decode())
        connection.close()

channel.basic_consume(
    queue=callback_queue,
    on_message_callback=on_response,
    auto_ack=True
)

channel.basic_publish(
    exchange='',
    routing_key='requests',
    properties=pika.BasicProperties(
        reply_to=callback_queue,
        correlation_id=corr_id
    ),
    body='Hello'
)

print("Waiting for response...")
channel.start_consuming()
