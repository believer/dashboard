let component = ReasonReact.statelessComponent("Navigation");

let make = (~displayGit, ~displayTrello, ~toggleGit, ~toggleTrello, _children) => {
  ...component,
  render: _self =>
    <div
      className="flex flex-column items-center bg-alabaster br h-100 b--black-10 pa4">
      <IconGitHub
        className={Cn.make(["mb4", "light-silver"->Cn.ifTrue(!displayGit)])}
        onClick=toggleGit
      />
      <IconBook
        className={Cn.make(["light-silver"->Cn.ifTrue(!displayTrello)])}
        onClick=toggleTrello
      />
    </div>,
};
