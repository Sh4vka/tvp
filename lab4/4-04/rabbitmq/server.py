import pika

connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()

channel.queue_declare(queue='requests')

def callback(ch, method, properties, body):
    print("Received:", body.decode())

    response = "Processed: " + body.decode()

    ch.basic_publish(
        exchange='',
        routing_key=properties.reply_to,
        properties=pika.BasicProperties(
            correlation_id=properties.correlation_id
        ),
        body=response
    )

    ch.basic_ack(delivery_tag=method.delivery_tag)

channel.basic_consume(queue='requests', on_message_callback=callback)

print("RabbitMQ server started...")
channel.start_consuming()
