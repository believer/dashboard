open Dom.Storage;

module SettingsForm = {
  open Formality;

  type field =
    | Token;

  type state = {token: string};
  type message = string;

  module TokenField = {
    let update = (_state, value) => {token: value};

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

  let validators = [TokenField.validator];
};

module SettingsFormContainer = Formality.Make(SettingsForm);

let component = ReasonReact.statelessComponent("GitHubSettings");

let make = _children => {
  ...component,
  render: _self =>
    <SettingsFormContainer
      initialState={
        token:
          switch (localStorage |> getItem("github")) {
          | Some(v) => v
          | None => ""
          },
      }
      onSubmit={
        (state, form) => {
          localStorage |> setItem("github", state.token);
          form.notifyOnSuccess(None);
        }
      }>
      ...{
           form =>
             <form onSubmit={form.submit->Formality.Dom.preventDefault}>
               <h2 className="f3 mt0 dark-gray">
                 "GitHub settings"->Utils.str
               </h2>
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
