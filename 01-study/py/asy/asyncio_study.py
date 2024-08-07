import asyncio
import time

async def say_after(delay, what):
    await asyncio.sleep(delay)
    print(what)

async def main():
    task1 = asyncio.create_task(
        say_after(1, 'hello'))

    task2 = asyncio.create_task(
        say_after(2, 'world'))
    print(f"started at {time.strftime('%X')}")
    await task1
    await task2
    print(f"finished at {time.strftime('%X')}")

async def main2():
    print(f"started at {time.strftime('%X')}")
    task = await asyncio.gather(
        say_after(1, 'hello'),
        say_after(2, 'world'))
    print(f"finished at {time.strftime('%X')}")

asyncio.run(main2())
