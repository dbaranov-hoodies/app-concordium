
from tests.application_client.boilerplate_command_sender import BoilerplateCommandSender
from tests.utils import navigate_until_text_and_compare


# In this test we send to the device a transaction to sign and validate it on screen
# The transaction is short and will be sent in one chunk
# We will ensure that the displayed information is correct by using screenshots comparison



@pytest.mark.active_test_scope
def test_sign_tx_simple_transfer_universal(
    backend, navigator, default_screenshot_path, test_name
):
    # Use the app interface instead of raw interface
    client = BoilerplateCommandSender(backend)
    # The path used for this entire test
    path: str = "m/44/919/0/0/0/0"

    # Create the transaction that will be sent to the device for signing
    transaction = "20a845815bd43a1999e90fbf971537a70392eb38f89e6bd32b3dd70e1a9551d7000000000000000a0000000000000064000000290000000063de5da70320a845815bd43a1999e90fbf971537a70392eb38f89e6bd32b3dd70e1a9551d7"
    transaction = bytes.fromhex(transaction)

    amount = 0xffffffffffffffff

    # Send the sign device instruction.
    # As it requires on-screen validation, the function is asynchronous.
    # It will yield the result when the navigation is done
    with client.sign_simple_transfer_universal_simple(path=path, transaction=transaction, amount=amount):
        # Validate the on-screen request by performing the navigation appropriate for this device
        navigate_until_text_and_compare(
            backend, navigator, "Sign", default_screenshot_path, test_name
        )

    # The device as yielded the result, parse it and ensure that the signature is correct
    response = client.get_async_response().data
    response_hex = response.hex()
    print("response", response_hex)
    assert (
        response_hex
        == "e5f112237d58f908c44385827e71048869db7e8f513e2ceb5da6a6370e2088f4371f93d6e08f9f6c1dd92c74fe565727b8f81600541e817d35cfeec4cc3bc408"
    )