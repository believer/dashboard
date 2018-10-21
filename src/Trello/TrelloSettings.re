open Dom.Storage;

module SettingsForm = {
  open Formality;

  type field =
    | Username
    | Key
    | Token;

  type state = {
    token: string,
    username: string,
    key: string,
  };
  type message = string;

  module TokenField = {
    let update = (state, value) => {...state, token: value};

    let validator = {
      field: Token,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({token}) =>
        switch (token) {
        | "" => Error("Token is required")
        | _ => Ok(Valid)
        },
    };
  };

  module UsernameField = {
    let update = (state, value) => {...state, username: value};

    let validator = {
      field: Username,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({username}) =>
        switch (username) {
        | "" => Error("Username is required")
        | _ => Ok(Valid)
        },
    };
  };

  module KeyField = {
    let update = (state, value) => {...state, key: value};

    let validator = {
      field: Key,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({key}) =>
        switch (key) {
        | "" => Error("Key is required")
        | _ => Ok(Valid)
        },
    };
  };

  let validators = [
    TokenField.validator,
    UsernameField.validator,
    KeyField.validator,
  ];
};

module SettingsFormContainer = Formality.Make(SettingsForm);

let component = ReasonReact.statelessComponent("TrelloSettings");

let make = _children => {
  ...component,
  render: _self =>
    <SettingsFormContainer
      initialState={
        username:
          switch (localStorage |> getItem("trello_username")) {
          | Some(v) => v
          | None => ""
          },
        key:
          switch (localStorage |> getItem("trello_key")) {
          | Some(v) => v
          | None => ""
          },
        token:
          switch (localStorage |> getItem("trello_token")) {
          | Some(v) => v
          | None => ""
          },
      }
      onSubmit={
        (state, form) => {
          localStorage |> setItem("trello_username", state.username);
          localStorage |> setItem("trello_key", state.key);
          localStorage |> setItem("trello_token", state.token);

          form.notifyOnSuccess(None);
        }
      }>
      ...{
           form =>
             <form onSubmit={form.submit->Formality.Dom.preventDefault}>
               <h2 className="f3 mt0 dark-gray">
                 "Trello settings"->Utils.str
               </h2>
               <Input
                 error={SettingsForm.Token->(form.result)}
                 label="Username"
                 id="username"
                 onChange={
                   event =>
                     form.change(
                       SettingsForm.Username,
                       SettingsForm.UsernameField.update(
                         form.state,
                         event->ReactEvent.Form.target##value,
                       ),
                     )
                 }
                 value={form.state.username}
               />
               <Input
                 error={SettingsForm.Token->(form.result)}
                 label="Key"
                 id="key"
                 onChange={
                   event =>
                     form.change(
                       SettingsForm.Key,
                       SettingsForm.KeyField.update(
                         form.state,
                         event->ReactEvent.Form.target##value,
                       ),
                     )
                 }
                 value={form.state.key}
               />
               <Input
                 error={SettingsForm.Token->(form.result)}
                 label="Token"
                 id="token"
                 onChange={
                   event =>
                     form.change(
                       SettingsForm.Token,
                       SettingsForm.TokenField.update(
                         form.state,
                         event->ReactEvent.Form.target##value,
                       ),
                     )
                 }
                 value={form.state.token}
               />
               {
                 switch (form.status) {
                 | Submitted =>
                   <div className="green mb2"> "Saved"->Utils.str </div>
                 | _ => ReasonReact.null
                 }
               }
               <Button type_="submit"> "Save"->Utils.str </Button>
             </form>
         }
    </SettingsFormContainer>,
};
