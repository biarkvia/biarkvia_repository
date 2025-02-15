<?php

namespace App\Controller;

use App\Entity\Account;
use App\Form\CreateAccountType;
use App\Form\RemoveType;
use App\Operation\AccountOperation;
use App\Operation\EmployeeOperation;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class EmployeeAccountController extends AbstractController
{
    private EmployeeOperation $employeeOperation;
    private AccountOperation $accountOperation;

    public function __construct(
        EmployeeOperation $employeeOperation,
        AccountOperation $accountOperation
    ) {
        $this->employeeOperation = $employeeOperation;
        $this->accountOperation = $accountOperation;
    }

    #[Route('/accounts/{employeeId}', name: 'employee_account', methods: ['GET'], requirements: ['employeeId' => '\d+'])]
    public function viewAccounts(int $employeeId, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);

        if (!$employee) {
            return $this->redirectToRoute('error', [
                'message' => 'Zaměstnanec nebyl nalezen.',
            ]);
        }

        $criteria = [
            'name' => $request->query->get('name'),
            'type' => $request->query->get('type'),
            'expiration' => $request->query->get('expiration'),
        ];

        $criteria = array_filter($criteria, fn($value) => !empty($value) && $value !== []);

        $accounts = $this->accountOperation->findByCriteria($criteria, $employee);
        $form = $this->createForm(CreateAccountType::class);

        return $this->render('account/employee_account.html.twig', [
            'accounts' => $accounts,
            'employee' => $employee,
            'form' => $form->createView(),
        ]);
    }

    #[Route('/accounts/{employeeId}/edit/{accountId}', name: 'edit_account', requirements: ['employeeId' => '\d+', 'accountId' => '\d+'])]
    public function edit(int $employeeId, int $accountId, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);

        if (!$employee) {
            return $this->redirectToRoute('error', ['message' => 'Zaměstnanec nebyl nalezen.']);
        }

        $account = $this->accountOperation->findById($accountId);

        if (!$account || $account->getEmployee()->getId() !== $employeeId) {
            return $this->redirectToRoute('error', ['message' => 'Účet zaměstnance nebyl nalezen.']);
        }

        $form = $this->createForm(CreateAccountType::class, $account, ['is_edit' => true]);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->accountOperation->store($account);
            return $this->redirectToRoute('employee_account', ['employeeId' => $employeeId]);
        }

        return $this->render('account/edit_account.html.twig', [
            'form' => $form->createView(),
            'account' => $account,
            'employee' => $employee,
        ]);
    }

    #[Route('/accounts/{employeeId}/remove/{accountId}', name: 'remove_account', requirements: ['employeeId' => '\d+', 'accountId' => '\d+'])]
    public function remove(int $employeeId, int $accountId, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);
        $account = $this->accountOperation->findById($accountId);

        if (!$employee || !$account || $account->getEmployee()->getId() !== $employeeId) {
            return $this->redirectToRoute('error', [
                'message' => 'Účet zaměstnance nebyl nalezen.',
            ]);
        }

        $form = $this->createForm(RemoveType::class, null, [
            'submit_label' => 'Smazat účet zaměstnance',
            'button_class' => 'btn-danger',
        ]);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->accountOperation->delete($account);
            return $this->redirectToRoute('employee_account', [
                'employeeId' => $employeeId,
                'message' => 'Účet byl úspěšně odstraněn.',
            ]);
        }

        return $this->render('account/remove_account.html.twig', [
            'form' => $form->createView(),
            'account' => $account,
            'employee' => $employee,
        ]);
    }

    #[Route('/accounts/{employeeId}/create', name: 'create_account', requirements: ['employeeId' => '\d+'])]
    public function create(int $employeeId, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);

        if (!$employee) {
            return $this->redirectToRoute('error', [
                'message' => 'Zaměstnanec nebyl nalezen.',
            ]);
        }

        $newAccount = new Account();
        $newAccount->setEmployee($employee);
        $form = $this->createForm(CreateAccountType::class, $newAccount, ['is_edit' => false]);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->accountOperation->store($newAccount);
            return $this->redirectToRoute('employee_account', [
                'employeeId' => $employee->getId(),
                'message' => 'Nový účet byl úspěšně vytvořen.',
            ]);
        }

        return $this->render('account/create_account.html.twig', [
            'accountForm' => $form->createView(),
            'employee' => $employee,
        ]);
    }
}
